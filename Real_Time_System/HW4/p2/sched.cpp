#include "sched.hpp"

bool
RM::RM_Comparator::operator()(const Task &lhs, const Task &rhs) const {
  return lhs.rate < rhs.rate;
}
