#pragma once

#include "Core/Common.h"

#include <thread>
#include <atomic>
#include "taskflow/taskflow.hpp"

namespace async
{
	extern tf::Executor executor;
}
