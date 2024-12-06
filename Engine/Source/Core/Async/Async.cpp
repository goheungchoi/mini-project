#include "Async.h"

#include <thread>

struct ThreadPool
{

  std::vector<std::thread> threads;

  std::vector<async::Task> tasks;

  void RecieveTask(const async::Task& taks)
  {
  }
};

static ThreadPool& GetThreadPool()
{
  static ThreadPool _threadPool;
  return _threadPool;
}

async::Task::Task() noexcept : _state{false}
{
}

async::Task::Task(const std::string& taskname) noexcept : _taskname(taskname), _state{false}
{
}

async::Task::Task(std::string&& taskname) noexcept : _taskname(std::move(taskname)), _state{false}
{
}



// Copy
async::Task::Task(const Task& other)
{

}

// Copy
async::Task& async::Task::operator=(const Task& other)
{
  if (this == &other)
    return *this;

	_taskname = other._taskname;
	_state.store(other._state);
  _taskContainer = other._taskContainer;


}

// Reset
async::Task::Task(Task&& other) noexcept
{
}

// Reset
async::Task& async::Task::operator=(Task&& other) noexcept
{
  // TODO: insert return statement here
}
