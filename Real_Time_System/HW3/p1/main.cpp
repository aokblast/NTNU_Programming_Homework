#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <functional>
#include <cassert>
#include <pthread.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <iostream>

constexpr auto TARGET_SIG = SIGUSR1;
constexpr int M = 1000000;
constexpr int GIGA = 1000000000;
constexpr int LIM = 5000;

using TaskFunc = void* (*)(void *);

std::vector<long long> t1_res, t2_res;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

class TimerTask {
private:
    std::vector<std::pair<TaskFunc, timer_t>> _tasks;
public:
    void
    register_work(TaskFunc work, size_t period_ns, size_t period_s = 0) {
        _tasks.push_back({work, timer_t()});
        auto &[_task, timer] = _tasks.back();

        sigevent evp;
        evp.sigev_value.sival_int = _tasks.size() - 1;
        evp.sigev_notify = SIGEV_SIGNAL;
        evp.sigev_signo = TARGET_SIG;
        assert(timer_create(CLOCK_REALTIME, &evp, &timer) == 0);

        struct itimerspec ts;
        ts.it_interval.tv_sec = period_s;
        ts.it_interval.tv_nsec = period_ns * M;
        ts.it_value.tv_sec = 1;
        ts.it_value.tv_nsec = 0;
        assert(timer_settime(timer, 0, &ts, nullptr) == 0);
    }

    void
    clear_timers() {
        for(const auto &[_task, timer] : _tasks)
            timer_delete(timer);
    }

    friend void
    task_issuer(int sig, siginfo_t *si, void *unused);
} tasks;

void
task_issuer(int sig, siginfo_t *si, void *unused) {
    pthread_t pt;
    assert(pthread_create(&pt, nullptr, tasks._tasks[si->si_value.sival_int].first, nullptr) == 0);
}

static void
set_RT_param(int policy, int priority) {
    sched_param sched;
    int old_policy;
    assert(pthread_getschedparam(pthread_self(), &old_policy, &sched) == 0);
    sched.sched_priority = priority;
    if(pthread_setschedparam(pthread_self(), policy, &sched))
        perror("setschedparam error");
}

static void
pin_CPU(int cpu_number) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu_number, &mask);

  if(sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
    perror("sched_setaffinity error");
    exit(EXIT_FAILURE);
  }
}

static void
set_signal_handler() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = task_issuer;
    sigemptyset(&sa.sa_mask);

    if(sigaction(TARGET_SIG, &sa, nullptr)) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }
}

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

void *
t1(void *) {
  set_RT_param(SCHED_FIFO, 99);

  auto time =  work_inner([&]() {
      for(int i = 0; i < 1500000; ++i)
          sqrt(i);
      pthread_mutex_lock(&mut);
      for(int i = 0; i < 1000000; ++i)
          sqrt(i);
      pthread_mutex_unlock(&mut);
  });


  t1_res.push_back(time);
  std::cout << (double)time / M << std::endl;
  return nullptr;
}

void *
t2(void *) {
  set_RT_param(SCHED_FIFO, 90);

  auto time = work_inner([](){
      for(int i = 0; i < 8000000; ++i)
          sqrt(i);
      pthread_mutex_lock(&mut);
      for(int i = 0; i < 2000000; ++i)
          sqrt(i);
      pthread_mutex_unlock(&mut);
  });

  t2_res.push_back(time);
  return nullptr;
}

void *
t3(void *) {
  set_RT_param(SCHED_FIFO, 95);

  auto time = work_inner([](){
      for(int i = 0; i < 10000000; ++i)
          sqrt(i);
  });

  return nullptr;
}


int
main() {
    pin_CPU(5);
    set_signal_handler();
    tasks.register_work(t1, 100);
    tasks.register_work(t2, 250);
    tasks.register_work(t3, 400);
    tasks.register_work(t3, 400);

    while(true) {
        pause();
        if(t1_res.size() >= LIM)
            break;
    }

    tasks.clear_timers();

    std::ofstream p1_res("p1_res"), p2_res("p2_res");

    for(const auto val: t1_res)
        p1_res << val << '\n';

    for(const auto val: t2_res)
        p2_res << val << '\n';
}
