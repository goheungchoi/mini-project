#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../RenderFrameworks/Shader.h"
#include "../Resources/Buffer.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/QuadFrame.h"
#include "Core/Common.h"

using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

class VinettePass
{
private:
  Device* _device;
  Quad::QuadFrame* _frame;

public:
  VinettePass(Device* device): _device { device }
  {
    _frame = new Quad::QuadFrame(device);
    _frame->InitializeBuffers();
  }
  ~VinettePass() { SAFE_RELEASE(_frame); }

  void QuadDraw()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    unsigned int stride = sizeof(Quad::Vertex);
    unsigned int offset = 0;
    dc->IASetVertexBuffers(0, 1, _frame->_vertexBuffer.GetAddressOf(), &stride,
                           &offset);
    dc->IASetIndexBuffer(_frame->_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _device->GetImmContext()->DrawIndexed(_frame->_indexCount, 0, 0);
  }
};