#pragma once

class IRenderer
{
public:
  virtual bool Init_Win32(void *hInstance, void *hwnd) = 0;

  virtual bool Cleanup() = 0;

  virtual void ResizeScreen(unsigned int width, unsigned int height) = 0;

  virtual void BeginFrame() = 0;

  virtual void BeginDraw() = 0;

  virtual void EndDraw() = 0;

  virtual void EndFrame() = 0;

  virtual void BindPipeline() = 0;

  virtual void BindResource() = 0;

  // 렌더러 안에서 D3D11 리소스 생성과 파괴
  virtual bool CreateMesh() = 0;
  virtual bool DestroyMesh() = 0;

  virtual bool CreateTexture() = 0;
  virtual bool DestroyTexture() = 0;

  virtual bool CreateShaderModule() = 0;
  virtual bool DestroyShaderModule() = 0;

  virtual bool CreatePipeline() = 0;
  virtual bool DestroyPipeline() = 0;

  virtual bool CreateComputeEffect() = 0;
  virtual bool DestoryComputeEffect() = 0;
};
