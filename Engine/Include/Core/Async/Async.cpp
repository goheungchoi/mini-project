#include "Core/Async/Async.h"

tf::Executor async::executor(std::thread::hardware_concurrency() * 0.75);

int i = 0;