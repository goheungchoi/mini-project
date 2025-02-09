// Random.cpp
#include "Core/Utils/RandomGenerator.h"

std::unique_ptr<Random> Random::_instance = std::make_unique<Random>();
