#pragma once
#include "Core/Common.h"
class BillboardQuad;
class Device;
class Billboard
{
public:
  Billboard(Device* device);
  ~Billboard();
  void SetPosition();
  void SetSize();
  
private:
  BillboardQuad* _quad{nullptr};
};