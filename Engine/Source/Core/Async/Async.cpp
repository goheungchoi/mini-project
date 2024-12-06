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
