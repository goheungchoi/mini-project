#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "Buffer.h"
/*
Current Frmae Buffer
:stores information about the current frame's constant buffers
*/

enum class MeshCBType
{
  World,
  PixelData,
  BoneMatrix,
  End
};

class FrameConstantBuffer
{
private:
  Device* _device;

public:
  ComPtr<ID3D11Buffer> _constantBuffer;

public:
  FrameConstantBuffer(Device* device) : _device{device}
  {
    _constantBuffer = _device->CreateConstantBuffer<Constant::Frame>();
  };

public:
  void UpdateContantBuffer(Constant::Frame cb)
  {
    _device->GetImmContext()->UpdateSubresource(_constantBuffer.Get(), 0,
                                                nullptr, &cb, 0, 0);
  }
};

class MeshConstantBuffer
{
private:
  Device* _device;

public:
  /**
   * @brief 
   * slot0 : world
   * slot1 : pixeldata
   * slot2 : boneMatirx
   */
  std::vector<ComPtr<ID3D11Buffer>> _constantBuffers;

public:
  MeshConstantBuffer(Device* device) : _device{device}
  {
    // 일단 5개 잡아두긴했는데 늘어나면 조정.
    _constantBuffers.resize(5);
    _constantBuffers[static_cast <UINT>(MeshCBType::World)] =
        _device->CreateConstantBuffer<Constant::World>();
    _constantBuffers[static_cast<UINT>(MeshCBType::PixelData)] =
        _device->CreateConstantBuffer<Constant::PixelData>();
    _constantBuffers[static_cast<UINT>(MeshCBType::BoneMatrix)] =
        _device->CreateConstantBuffer<Constant::BoneMatrix>();
  }
  ~MeshConstantBuffer() = default;

public:
  template <typename T>
  void UpdateContantBuffer(T cb, MeshCBType type)
  {
    _device->GetImmContext()->UpdateSubresource(
        _constantBuffers[static_cast<UINT>(type)].Get(), 0, nullptr, &cb, 0, 0);
  }
};
