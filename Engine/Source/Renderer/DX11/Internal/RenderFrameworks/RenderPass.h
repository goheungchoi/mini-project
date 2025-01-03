#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "Core/Common.h"

class RenderPassManager
{
private:
  //renderer에서 생성한 deive 참조.
  Device* _device = nullptr;

public:
  RenderPassManager(Device* device) : _device{device} {}
  ~RenderPassManager() {}

public:

  // 활성화된 렌더 패스 처리
  void ProcessRenderPass()
  {
    /*if (IsRenderPassActive(RenderPassType::Opaque))
    {
      SetOpaque();
    }
    if (IsRenderPassActive(RenderPassType::Transparent))
    {
      SetTransparent();
    }
    if (IsRenderPassActive(RenderPassType::Light))
    {
      SetLight();
    }
    if (IsRenderPassActive(RenderPassType::Shadow))
    {
      SetShadow();
    }*/
  }

private:
  void SetOpaque()
  {
  }

  void SetTransparent()
  {
  }

  void SetLight()
  {
  }

  void SetShadow()
  {
  }
};