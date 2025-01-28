#pragma once
#include "PostProcess.h"

class OutLineProcess :public PostProcess
{
public:
  OutLineProcess(Device* device):PostProcess(device){}
  virtual ~OutLineProcess() {}
};