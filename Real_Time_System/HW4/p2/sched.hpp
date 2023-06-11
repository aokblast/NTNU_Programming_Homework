#pragma once

#include <queue>
#include <vector>

using task_t = void* (*)(void *);

class RM {
public:
  struct Task;
private:
  class RM_Comparator {
  public:
    bool
    operator()(const Task &lhs, const Task &rhs) const;
  };

public:
  struct Task {
    task_t task;
    double rate;
  };
  std::priority_queue<Task, std::vector<Task>, RM_Comparator> q;
};

class FCFS {
public:
  std::queue<task_t> q;
};
