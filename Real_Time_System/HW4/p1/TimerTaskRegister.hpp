#pragma once
#include "sched.hpp"
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <csignal>

class TimerTaskRegister {
private:
  std::vector<std::pair<task_t, timer_t>> _tasks;
public:
  int
  register_work(task_t work, size_t period_ns, size_t period_s);
  void
  clear_timers() const;
  inline std::vector<std::pair<task_t, timer_t>>
  get_tasks() const {
    return _tasks;
  }
};
