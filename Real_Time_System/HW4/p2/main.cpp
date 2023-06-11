#include "sched.hpp"
#include "Utility.hpp"
#include "TimerTaskRegister.hpp"
#include <sched.h>
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
#include <tuple>
#include <memory>

constexpr int M = 1000000;
constexpr int GIGA = 1000000000;

static TimerTaskRegister tasks;
static RM rm_tasks;
static FCFS fcfs_tasks;

std::mutex mut;
std::vector<long long> v1, v2, v3;

using PMC = std::tuple<int, pthread_mutex_t, pthread_cond_t>;
struct Deferrable_Task {
  pthread_t thread;
  PMC value;
  int rem;
};

std::deque<Deferrable_Task> def_tasks;

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
  set_RT_param(SCHED_FIFO, 99);
  v1.push_back(work_inner([&]() {
    for (int i = 0; i < 95000000; ++i)
      sqrt(i);
  }));
  return nullptr;
}

static void *
t2(void *) {
  set_RT_param(SCHED_FIFO, 99);
  v2.push_back(work_inner([&]() {
    for (int i = 0; i < 47500000; ++i)
      sqrt(i);
  }));
  return nullptr;
}

static void *
t3(void *param) {
  set_RT_param(SCHED_FIFO, 99);
  long long res = 0;
  PMC *value = (PMC *)param;
  int tot = 3;
  auto time_elapse = work_inner([&](){
    while(tot--) {
      pthread_mutex_lock(&std::get<1>(*value));
      while(std::get<0>(*value) == 0) {
        pthread_cond_wait(&std::get<2>(*value), &std::get<1>(*value));
      }
      for(int i = 0; i < 500000; ++i)
        sqrt(i);
      pthread_mutex_unlock(&std::get<1>(*value));
    }});
  v3.push_back(time_elapse);
  return nullptr;
}

static void *
deferable_task(void *) {
  set_RT_param(SCHED_FIFO, 98);
  int quota = 10;

  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC, &start)) {
    perror("clock_getres error");
    exit(EXIT_FAILURE);
  }

  while(quota) {
    if(def_tasks.size()) {
      const std::lock_guard<std::mutex> lock(mut);
      auto &front = def_tasks.front();

      int alloc = std::min(quota, front.rem);
      front.rem -= alloc;
      quota -= alloc;
      while(alloc--) {
        pthread_mutex_lock(&std::get<1>(front.value));
        std::get<0>(front.value) = 1;
        pthread_cond_signal(&std::get<2>(front.value));
        pthread_mutex_unlock(&std::get<1>(front.value));
      }
      if(front.rem == 0)
        def_tasks.pop_front();
    } else {
      if (clock_gettime(CLOCK_MONOTONIC, &end)) {
        perror("clock_getres error");
        exit(EXIT_FAILURE);
      }
      if((end.tv_nsec - start.tv_nsec) >= 10000000)
        break;
    }
  }
  return nullptr;
}


static void *
t3_issuer(void *) {
  const std::lock_guard<std::mutex> lock(mut);
  def_tasks.push_back({pthread_t(), {0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}, 3});
  // pthread_mutex_lock(&std::get<1>(def_tasks.back().value));
  pthread_create(&def_tasks.back().thread, nullptr, t3, &def_tasks.back().value);
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
  tasks.register_work(t1, 500, 0);
  tasks.register_work(t2, 700, 0);
  tasks.register_work(deferable_task, 40, 0);

  while(true) {
    pause();
  }
}
