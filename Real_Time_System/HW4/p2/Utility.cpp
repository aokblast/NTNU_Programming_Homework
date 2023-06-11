#include "Utility.hpp"
#include <sched.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <pthread.h>

void
pin_CPU(int cpu_number) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu_number, &mask);

  if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
    perror("sched_setaffinity error");
    exit(1);
  }
}

void set_RT_param(int policy, int priority) {
  sched_param sched;
  int old_policy;
  assert(pthread_getschedparam(pthread_self(), &old_policy, &sched) == 0);
  sched.sched_priority = priority;
  if (pthread_setschedparam(pthread_self(), policy, &sched))
    perror("setschedparam error");
}
