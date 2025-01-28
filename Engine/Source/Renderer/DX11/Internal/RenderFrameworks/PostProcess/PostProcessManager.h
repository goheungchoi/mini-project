#pragma once
#include "../../Common.h"
#include "../../Device.h"
class PostProcessManager
{
private:
  Device* _device = nullptr;

public:
  PostProcessManager(Device* device) : _device{ device }
  {
    
  }
};