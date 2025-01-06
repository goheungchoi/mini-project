#pragma once
#include "Core/Common.h"
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/Buffer.h"
class RenderPassManager
{
private:
  std::vector<MeshBuffer*> _transparentMeshes;
  std::vector<MeshBuffer*> _shadowMesh;
  std::vector<MeshBuffer*> _opaqueMesh;

private:
  // renderer에서 생성한 deive 참조.
  Device* _device = nullptr;

public:
  RenderPassManager(Device* device) : _device{device} {}
  ~RenderPassManager() {}

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
    
  }
};