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
  WVP,
  End
};

class FrameBuffer
{
private:
  Device* _device;

public:
  /**
   * @brief slot0 : currentWVP
   */
  std::vector<ComPtr<ID3D11Buffer>> _constantBuffers;

public:
  FrameBuffer(Device* device) : _device{device}
  {
    // 일단 5개 잡아두긴했는데 늘어나면 조정.
    _constantBuffers.resize(5);
    _constantBuffers[0] = _device->CreateConstantBuffer<Constant::WVP>();
  }
  ~FrameBuffer() = default;

public:
  template <typename T>
  void UpdateContantBuffer(T cb, CBType type)
  {
    _device->GetImmContext()->UpdateSubresource(
        _constantBuffers[static_cast<UINT>(type)].Get(), 0, nullptr, &cb, 0, 0);
  }
};
