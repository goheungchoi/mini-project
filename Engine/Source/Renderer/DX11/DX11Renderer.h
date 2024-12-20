#pragma once
#include "../IRenderer.h"
#include "Core/Common.h"
class Device;
class SwapChain;
#ifdef _DEBUG
class DebugLayer;
#endif // _DEBUG
struct ResourceStorage;


class DX11Renderer : public IRenderer
{
public:
  DX11Renderer() = default;
  virtual ~DX11Renderer();

public:
  // IRenderer을(를) 통해 상속됨
  bool Init_Win32(int width, int height, void* hInstance, void* hwnd) override;
  bool Cleanup() override;
  void ResizeScreen(unsigned int width, unsigned int height) override;
  void BeginFrame() override;
  void BeginDraw() override;
  void EndDraw() override;
  void EndFrame() override;
  void BindPipeline() override;
  void BindResource() override;
  bool CreateMesh(MeshHandle handle) override;
  bool DestroyMesh() override;
  bool CreateTexture() override;
  bool DestroyTexture() override;
  bool CreateShaderModule(ShaderHandle shaderHandle) override;
  bool DestroyShaderModule() override;
  bool CreatePipeline() override;
  bool DestroyPipeline() override;
  bool CreateComputeEffect() override;
  bool DestoryComputeEffect() override;
  
public:
 

private:
  
  Device* _device = nullptr;
  SwapChain* _swapChain = nullptr;
#ifdef _DEBUG
  DebugLayer* _debugLayer = nullptr;
#endif // _DEBUG
  ResourceStorage* _storage = nullptr;
};