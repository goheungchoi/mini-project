#pragma once
#include "../IRenderer.h"
struct ResourceStorage;
#ifdef _DEBUG
class DebugLayer;
#endif // _DEBUG
class Device;
class SwapChain;
class PipeLine;
class RenderPassManager;
class DX11Renderer : public IRenderer
{
public:
  DX11Renderer() {};
  virtual ~DX11Renderer();

public:
  // IRenderer을(를) 통해 상속됨
  bool Init_Win32(int width, int height, void* hInstance, void* hwnd) override;
  bool Cleanup() override;
  void ResizeScreen(unsigned int width, unsigned int height) override;
  void BeginFrame() override;
  void BeginDraw(MeshHandle handle, Matrix world) override;
  void DrawMesh(MeshHandle handle) override;
  void EndDraw() override;
  void EndFrame() override;
  void AddRenderPass(MeshHandle handle, RenderPassType type) override;
  void DeleteRenderPass(MeshHandle handle, RenderPassType type) override;
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
private:
  Device* _device = nullptr;
  SwapChain* _swapChain = nullptr;
#ifdef _DEBUG
  DebugLayer* _debugLayer = nullptr;
#endif // _DEBUG
  ResourceStorage* _storage = nullptr;
  PipeLine* _pso = nullptr;
  RenderPassManager* _passMgr = nullptr;
};