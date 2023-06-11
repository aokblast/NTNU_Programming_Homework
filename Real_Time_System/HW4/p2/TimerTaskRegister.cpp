#include "TimerTaskRegister.hpp"
#include <cassert>
#include <ctime>

constexpr int M = 1000000;

int
TimerTaskRegister::register_work(task_t work, size_t period_ns,
                                     size_t period_s = 0) {
  _tasks.push_back({work, timer_t()});
  auto &[_task, timer] = _tasks.back();

  sigevent evp;
  evp.sigev_value.sival_int = _tasks.size() - 1;
  evp.sigev_notify = SIGEV_SIGNAL;
  evp.sigev_signo = SIGUSR1;
  assert(timer_create(CLOCK_REALTIME, &evp, &timer) == 0);

  struct itimerspec ts;
  ts.it_interval.tv_sec = period_s;
  ts.it_interval.tv_nsec = period_ns * M;
  ts.it_value.tv_sec = 1;
  ts.it_value.tv_nsec = 0;
  assert(timer_settime(timer, 0, &ts, nullptr) == 0);
  return _tasks.size() - 1;
}

void
TimerTaskRegister::clear_timers() const {
  for(const auto &[_task, timer] : _tasks)
    timer_delete(timer);
}


