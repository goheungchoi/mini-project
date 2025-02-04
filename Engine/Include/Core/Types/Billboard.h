#pragma once
#include "Core/Common.h"
#include "Core/Handle/ResourceHandle.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
class BillboardQuad;
class Device;
class Billboard
{
public:
  Billboard(Device* device);
  ~Billboard();
  void SetPosition(Vector4 position);
  void SetSize(Vector3 size);
  void SetTexture(TextureHandle texHandle);

public:
  BillboardQuad* _quad{nullptr};
};