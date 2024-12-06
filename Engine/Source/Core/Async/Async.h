#pragma once

#include <atomic>

#include "Core/Common.h"

// TODO: Goheung Choi

namespace async
{

template <typename T>
class Future
{
public:
  void Wait()
  {
  }

  void WaitFor(uint64_t millisecond)
  {
  }
};

template <typename... Ts>
class MultiFuture
{
public:
  void WaitAny()
  {
  }

  void WaitAnyForAll(uint64_t millisecond)
  {
  }

  void WaitAll()
  {
  }

  void WaitAllFor(uint64_t millisecond)
  {
  }
};

class Task
{
  std::atomic<bool> _state;

  std::function<void()> _taskContainer;

	std::any _retVal;

public:
  Task() noexcept;

  Task(Task &&other) noexcept;
  Task &operator=(Task &&other) noexcept;

  template <typename Ret>
  Future<Ret> GetFuture()
  {
  }

  bool IsReady()
  {
    return _state;
  }

  template <typename Ret, typename... Args>
  static Task CreateTask(std::function<Ret(Args...)> callable)
  {
  }

  template <typename Ret, std::invocable Func, typename... Args>
  static Task CreateTask(Func f, Args... args)
  {
  }
};

class TaskGroup
{
public:
};

template <typename Ret, typename... Args>
Future<Ret> SubmitTask(std::function<Ret(Args...)> &&callable)
{
}

template <typename Ret, std::invocable Func, typename... Args>
Future<Ret> SubmitTask(Func f, Args... args)
{
}

template <std::invocable Func, typename... Args>
void DispatchTask(Func f, Args... args);

void WaitIdle();

} // namespace async
