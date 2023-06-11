#include "sched.hpp"
#include "Utility.hpp"
#include "TimerTaskRegister.hpp"
#include <signal.h>
#include <pthread.h>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <mutex>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>

constexpr int M = 1000000;
constexpr int GIGA = 1000000000;

static TimerTaskRegister tasks;
static RM rm_tasks;
static FCFS fcfs_tasks;

std::mutex mut1, mut2;

std::vector<long long> v1, v2, v3;

static inline long long
work_inner(std::function<void(void)> func) {
  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC, &start)) {
    perror("clock_getres error");
    exit(EXIT_FAILURE);
  }

  func();

  if (clock_gettime(CLOCK_MONOTONIC, &end)) {
    perror("clock_getres error");
    exit(EXIT_FAILURE);
  }

  const long long sec = end.tv_sec - start.tv_sec;
  const long long nsec = end.tv_nsec - start.tv_nsec;

  return static_cast<long long>(sec * GIGA + nsec);
}

static void
task_issuer(int sig, siginfo_t *si, void *unused) {
  pthread_t pt;
  assert(pthread_create(&pt, nullptr, tasks.get_tasks()[si->si_value.sival_int].first, nullptr) == 0);
}

static void *
t1(void *) {
  v1.push_back(work_inner([&]() {
    for (int i = 0; i < 95000000; ++i)
      sqrt(i);
  }));
  return nullptr;
}

static void *
t2(void *) {
  v2.push_back(work_inner([&]() {
    for (int i = 0; i < 47500000; ++i)
      sqrt(i);
  }));
  return nullptr;
}

static void *
t3(void *) {
  v3.push_back(work_inner([&]() {
    for (int i = 0; i < 1500000; ++i)
      sqrt(i);
  }));
  return nullptr;
  }

static void *
t1_issuer(void *) {
  const std::lock_guard<std::mutex> lock1(mut1);
  rm_tasks.q.push({t1, 200.0 / 500.0});
  return nullptr;
}

static void *
t2_issuer(void *) {
  const std::lock_guard<std::mutex> lock1(mut1);
  rm_tasks.q.push({t2, 100.0 / 700.0});
  return nullptr;
}

static void *
t3_issuer(void *) {
  const std::lock_guard<std::mutex> lock2(mut2);
  fcfs_tasks.q.push(t3);
  return nullptr;
}

static void
sig_int_handler(int signum) {
  t3_issuer(nullptr);
}

static void
sig_abrt_handler(int signum) {
  tasks.clear_timers();
  std::ofstream t1_res("t1_res"), t2_res("t2_res"), t3_res("t3_res");

  for(const auto val: v1)
    t1_res << val << std::endl;

  for(const auto val: v2)
    t2_res << val << std::endl;

  for(const auto val: v3)
    t3_res << val << std::endl;

  signal(SIGINT, SIG_DFL);
  signal(SIGABRT, SIG_DFL);
  exit(0);
}

static void
set_signal_handler() {
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = task_issuer;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, nullptr)) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }
  signal(SIGINT, sig_int_handler);
  signal(SIGABRT, sig_abrt_handler);
}

int
main() {
  pin_CPU(4);
  set_signal_handler();
  tasks.register_work(t1_issuer, 500, 0);
  tasks.register_work(t2_issuer, 700, 0);

  while(true) {
    [&]() {
      while(rm_tasks.q.size()) {
        const std::lock_guard<std::mutex> lock1(mut1);
        auto task = rm_tasks.q.top();
        rm_tasks.q.pop();
        task.task(nullptr);
      }
    }();
    [&]() {
      while (fcfs_tasks.q.size()) {
        const std::lock_guard<std::mutex> lock2(mut2);
        if(rm_tasks.q.size())
          return;
        auto task = fcfs_tasks.q.front();
        fcfs_tasks.q.pop();
        task(nullptr);
      }
    }();
  }
}
