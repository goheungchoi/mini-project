#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "Core/Common.h"

class RenderPassManager
{
private:
  //renderer���� ������ deive ����.
  Device* _device = nullptr;

public:
  RenderPassManager(Device* device) : _device{device} {}
  ~RenderPassManager() {}

public:

  // Ȱ��ȭ�� ���� �н� ó��
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