#pragma once

#include <atomic>

#include "Core/Common.h"

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
  std::tuple<Future<Ts>...> _futures;

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
  std::string _taskname;

  std::atomic<bool> _state;

  std::function<void()> _taskContainer;

  std::any _retVal;

public:
  Task() noexcept;
  Task(const std::string&) noexcept;
  Task(std::string&&) noexcept;

	Task(const Task& other);
  Task& operator=(const Task& other);

  Task(Task&& other) noexcept;
  Task& operator=(Task&& other) noexcept;

  template <typename Ret>
  Future<Ret> GetFuture()
  {
  }

  bool IsReady()
  {
    return _state;
  }

	void Reset()
  {
    _state = false;
    _taskContainer = nullptr;
    _retVal.reset();
	}

	void Notify()
  {
    _state = true;
	}

  template <typename Ret>
  static Task CreateTask(const std::string& taskName, std::function<Ret()>&& callable)
  {
    Task task;
    task._taskContainer = [callable]() -> void { 
			_retVal = callable(); 
		};
    return task;
  }

	template <>
  static Task CreateTask<void>(const std::string& taskName, std::function<void()>&& callable)
  {
    Task task;
    task._taskContainer = [callable]() -> void { callable(); };
    return task;
	}

  template <typename Ret, std::invocable Func, typename... Args>
  static Task CreateTask(const std::string& taskName, Func f, Args&&... args)
  {
    auto funcWithArgs = [f, ]

    Task task;
    task._taskContainer = [f]() -> void {

    };
  }
};

class TaskGroup
{
public:
};

void Submit(Task task);

template <typename Ret>
Future<Ret> SubmitTask(std::function<Ret()>&& callable)
{
}

template <typename Ret, std::invocable Func, typename... Args>
Future<Ret> SubmitTask(Func f, Args... args)
{
}

template <typename Ret>
void DispatchTask(std::function<Ret()>&& callable)
{
}

template <std::invocable Func, typename... Args>
void DispatchTask(Func f, Args... args);

void WaitIdle();

} // namespace async
