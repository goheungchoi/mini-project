#pragma once
#include "../IRenderer.h"
class Renderer:public IRenderer 
{
 public:
  Renderer() = default;
  virtual ~Renderer();
  // IRenderer��(��) ���� ��ӵ�
  bool Init_Win32(void* hInstance, void* hwnd) override;
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
};