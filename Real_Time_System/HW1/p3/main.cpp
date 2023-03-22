#define _GNU_SOURCE

#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sched.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <cmath>


constexpr int M = 1000000;

#ifdef T1
constexpr int TOTAL_CNT = 13000;
#else
constexpr int TOTAL_CNT = 2600;
#endif

constexpr int GIGA = 1000000000;

int idx = 0;

std::vector<long long> result_array(TOTAL_CNT);

void workload() {
  long j = 0;
  #ifdef T1
  for(long i = 0; i < 11345000; ++i)
    j = sqrt(i * i);
  #else
  for(long i = 0; i < 18000000; ++i)
    j = sqrt(i * i);
  #endif
}

static void
handle_timeout(int sig, siginfo_t *si, void *unused) {
  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC_RAW, &start)) {
    perror("clock_getres error");
    exit(EXIT_FAILURE);
  }

  workload();

  if (clock_gettime(CLOCK_MONOTONIC_RAW, &end)) {
    perror("clock_getres error");
    exit(EXIT_FAILURE);
  }

  const long long sec = end.tv_sec - start.tv_sec;
  const long long nsec = end.tv_nsec - start.tv_nsec;

  result_array[idx++] = static_cast<long long>(sec * GIGA + nsec);
}

void pinCPU(int cpu_number) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu_number, &mask);

  if(sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
    perror("sched_setaffinity error");
    exit(EXIT_FAILURE);
  }
}

int main(int argv, char *argc[]) {
  pinCPU(1);

  sigevent evp;
  struct sigaction sa;

  // Set signal structure
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = handle_timeout;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, nullptr)) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }

  // Associate timer structure
  timer_t timer;
  evp.sigev_value.sival_ptr = &timer;
  evp.sigev_notify = SIGEV_SIGNAL;
  evp.sigev_signo = SIGUSR1;

  if(timer_create(CLOCK_REALTIME, &evp, &timer)) {
    perror("timer_create error");
    exit(EXIT_FAILURE);
  }

  // Set timer
  struct itimerspec ts;
  ts.it_interval.tv_sec = 0;
  #ifdef T1
  ts.it_interval.tv_nsec = 100 * M;
  #else
  ts.it_interval.tv_nsec = 500 * M;
  #endif

  ts.it_value.tv_sec = 1;
  ts.it_value.tv_nsec = 0;

  workload();

  if(timer_settime(timer, 0, &ts, nullptr)) {
    perror("timer_settime error");
    exit(EXIT_FAILURE);
  }

  int cnt = TOTAL_CNT;

  while(cnt--) {
    pause();
  }

  std::ofstream fs(argc[1]);

  for(const auto time : result_array)
    fs << time << '\n';

  return 0;
}

