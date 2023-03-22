#include <time.h>
#include <stdio.h>
#include <math.h>

int main() {
  struct timespec bef, aft;
  int i = 0;
  clock_gettime(CLOCK_REALTIME, &bef);

  for (i = 1; i < 100000; i++) {
    (sqrt(i) + 1) * sqrt(i);
  }

  clock_gettime(CLOCK_REALTIME, &aft);

  printf("%ld\n", 1000000000 * (aft.tv_sec - aft.tv_sec) + (aft.tv_nsec - bef.tv_nsec));
}
