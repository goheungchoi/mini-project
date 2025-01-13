#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "Buffer.h"
/*
Current Frmae Buffer
:stores information about the current frame's constant buffers
*/

enum class CBType
{
  Frame,
  World,
  Defferd,
  Light,
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
   * @brief slot0 : currentWVP
   */
  std::vector<ComPtr<ID3D11Buffer>> _constantBuffers;

public:
  MeshConstantBuffer(Device* device) : _device{device}
  {
    // �ϴ� 5�� ��Ƶα��ߴµ� �þ�� ����.
    _constantBuffers.resize(5);
    _constantBuffers[static_cast <UINT>(CBType::World)] =
        _device->CreateConstantBuffer<Constant::World>();
  }
  ~MeshConstantBuffer() = default;

public:
  template <typename T>
  void UpdateContantBuffer(T cb, CBType type)
  {
    _device->GetImmContext()->UpdateSubresource(
        _constantBuffers[static_cast<UINT>(type)].Get(), 0, nullptr, &cb, 0, 0);
  }
};
