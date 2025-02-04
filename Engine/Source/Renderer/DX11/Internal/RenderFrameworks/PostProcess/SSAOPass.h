#pragma once
#include "Core/Common.h"
#include "Renderer/DX11/Internal/Common.h"
#include "Renderer/DX11/Internal/Resources/Buffer.h"
#include "Renderer/DX11/Internal/Resources/ConstantBuffer.h"
#include "Renderer/DX11/Internal/Resources/QuadFrame.h"
#include "Renderer/DX11/Internal/Device.h"
class SSAOPass
{
private:
  Quad::QuadFrame* _quad = nullptr;
  Device* _device = nullptr;

public:
  SSAOPass(Device* device) : _device{device} 
  { 
    _quad = new Quad::QuadFrame(device);
    
  }
  ~SSAOPass() {}

public:

};