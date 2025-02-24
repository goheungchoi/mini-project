#pragma once
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

class Device
{
public:
  Device() = default;
  ~Device() = default;
  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;
  Device(Device&&) = delete;
  Device& operator=(Device&&) = delete;

public:
  void Init()
  {
    UINT deviceFlags = 0;
    deviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    const D3D_FEATURE_LEVEL featurelevel[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_2,
        D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_1,
    };
    // create device and immediate Context
    HR_T(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                           deviceFlags, featurelevel, 1, D3D11_SDK_VERSION,
                           _device.GetAddressOf(), nullptr,
                           _immediateContext.GetAddressOf()));
  }

  ComPtr<ID3D11Buffer> CreateConstantBuffer(const void* data, UINT size) const
  {
    D3D11_SUBRESOURCE_DATA bufferData{};
    bufferData.pSysMem = data;

    ComPtr<ID3D11Buffer> buffer;
    const D3D11_SUBRESOURCE_DATA* bufferDataPtr = data ? &bufferData : nullptr;
    D3D11_BUFFER_DESC desc;
    desc =
        CreateBufferDesc(size, D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER);
    HR_T(_device->CreateBuffer(&desc, bufferDataPtr, buffer.GetAddressOf()));
    return buffer;
  }

  template <typename T>
  ComPtr<ID3D11Buffer> CreateConstantBuffer(const T* data = nullptr) const
  {
    static_assert(sizeof(T) % 16 == 0,
                  "must be 16-byte aligned for constant buffer creation.");
    return CreateConstantBuffer(data, sizeof(T));
  }

  /*
   *data -> vector::data()
   *size -> sizeof(struct)*vector::size()
   *buffer Type -> index,vertex
   */
  ComPtr<ID3D11Buffer> CreateDataBuffer(const void* data, UINT size,
                                        D3D11_BIND_FLAG bufferType,UINT structSize=0) const
  {
    D3D11_SUBRESOURCE_DATA bufferData{};
    bufferData.pSysMem = data;

    ComPtr<ID3D11Buffer> buffer;
    const D3D11_SUBRESOURCE_DATA* bufferDataPtr = data ? &bufferData : nullptr;
    D3D11_BUFFER_DESC desc;
    desc = CreateBufferDesc(size, D3D11_USAGE_DEFAULT, bufferType, structSize);
    HR_T(_device->CreateBuffer(&desc, bufferDataPtr, buffer.GetAddressOf()));
    return buffer;
  }

  ComPtr<ID3D11InputLayout> CreateInputLayout(
      const std::vector<D3D11_INPUT_ELEMENT_DESC> descs,
      const std::vector<uint8_t>& vsdata)
  {
    ComPtr<ID3D11InputLayout> layout;
    _device->CreateInputLayout(descs.data(), static_cast<UINT>(descs.size()),
                               vsdata.data(), vsdata.size(),layout.GetAddressOf());
    return layout;
  }
  ComPtr<ID3D11ShaderResourceView> CreateStructuredSRV(ID3D11Buffer* buffer, UINT elementCount)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format =
        DXGI_FORMAT_UNKNOWN; // Structured Buffer의 경우 UNKNOWN으로 설정
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = elementCount;

    ComPtr<ID3D11ShaderResourceView> srv;
    HR_T(_device->CreateShaderResourceView(buffer, &srvDesc,
                                           srv.GetAddressOf()));
    return srv;
  }

public:
  ID3D11Device* GetDevice() { return _device.Get(); }
  ID3D11DeviceContext* GetImmContext() { return _immediateContext.Get(); }

private:
  ComPtr<ID3D11Device> _device = nullptr;
  ComPtr<ID3D11DeviceContext> _immediateContext = nullptr;
};
