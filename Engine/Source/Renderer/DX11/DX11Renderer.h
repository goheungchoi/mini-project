#pragma once
#include "../IRenderer.h"

class Device;

class DX11Renderer : public IRenderer
{
public:
  DX11Renderer() = default;
  virtual ~DX11Renderer();
  // IRenderer을(를) 통해 상속됨
  bool Init_Win32(int width, int height, void *hInstance, void *hwnd) override;
  bool Cleanup() override;
  void ResizeScreen(unsigned int width, unsigned int height) override;
  void BeginFrame() override;
  void BeginDraw() override;
  void EndDraw() override;
  void EndFrame() override;
  void BindPipeline() override;
  void BindResource() override;
  bool CreateMesh() override;
  bool DestroyMesh() override;
  bool CreateTexture() override;
  bool DestroyTexture() override;
  bool CreateShaderModule() override;
  bool DestroyShaderModule() override;
  bool CreatePipeline() override;
  bool DestroyPipeline() override;
  bool CreateComputeEffect() override;
  bool DestoryComputeEffect() override;

private:
  Device *pDevice = nullptr;
};