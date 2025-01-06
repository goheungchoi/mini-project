#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Resources/Buffer.h"
#include "../Resources/PipeLineState.h"
#include "../Types.h"
#include "Core/Common.h"
class RenderPassManager
{
private:
  std::vector<MeshBuffer*> _transparentMeshes;
  std::vector<MeshBuffer*> _shadowMesh;
  std::vector<MeshBuffer*> _opaqueMesh;
  FrameBuffer* _frameBuffer;

private:
  // renderer에서 생성한 deive 참조.
  Device* _device;

public:
  RenderPassManager(Device* device) : _device{device}
  {
    _frameBuffer = new FrameBuffer(device);
  }
  ~RenderPassManager() { SAFE_RELEASE(_frameBuffer); }

public:
  void ClassfyPass(MeshBuffer* buff)
  {
    if (buff->flags & RenderPassType::Opaque)
    {
      _opaqueMesh.push_back(buff);
    }
    if (buff->flags & RenderPassType::Transparent)
    {
      _transparentMeshes.push_back(buff);
    }

    if (buff->flags & RenderPassType::Shadow)
    {
      _shadowMesh.push_back(buff);
    }
    if (buff->flags & RenderPassType::Light)
    {
    }
  }
  void ProcessPass()
  {
    // transparent -> forward rendering
    std::ranges::for_each(_transparentMeshes, [this](MeshBuffer* buffer) {
      _device->GetImmContext()->IASetVertexBuffers(
          0, 1, buffer->vertexBuffer.GetAddressOf(), &(buffer->stride),
          &(buffer->offset));
      _device->GetImmContext()->IASetIndexBuffer(buffer->indexBuffer.Get(),
                                                 DXGI_FORMAT_R32_UINT, 0);
    });
    //TODO : frame Buffer 작업 계속하기
  }
};