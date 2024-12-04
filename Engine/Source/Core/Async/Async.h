#pragma once

#include "Core/Common.h"

// TODO: Goheung Choi

namespace async {

class Task {};

class TaskGroup {};

void Execute(const Task& task);
void Execute(const TaskGroup& group);
}  // namespace async
