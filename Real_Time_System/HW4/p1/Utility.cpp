#include "Utility.hpp"
#include <sched.h>
#include <cstdlib>
#include <cstdio>

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
