#pragma once
#include "Core/Handle/ResourceHandle.h"
#include "DX11/Internal/Types.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
class IRenderer
{
public:
  virtual bool Init_Win32(int width, int height, void* hInstance,
                          void* hwnd) = 0;

  virtual bool Cleanup() = 0;

  virtual void ResizeScreen(unsigned int width, unsigned int height) = 0;
  /*
   * call per frame
   */
  virtual void BeginFrame() = 0;
  /*
   * call per mesh
   */
  virtual void BeginDraw(MeshHandle handle, Matrix world) = 0;
  /*
   * call per mesh
   */
  virtual void DrawMesh(MeshHandle handle) = 0;
  /*
   * call per mesh
   */
  virtual void EndDraw() = 0;
  /*
   * call per frame
   */
  virtual void EndFrame() = 0;

  /*
   * you can add more than one
   * call before begin draw
   * don't call every loop
   * Param1 : own meshHandle
   * Parma2 : RenderPassType::...
   */
  virtual void AddRenderPass(MeshHandle handle, RenderPassType type) = 0;
  virtual void DeleteRenderPass(MeshHandle handle, RenderPassType type) = 0;

  virtual void BindPipeline() = 0;

  virtual void BindResource() = 0;

  // 렌더러 안에서 D3D11 리소스 생성과 파괴
  virtual bool CreateMesh(MeshHandle handle) = 0;
  virtual bool DestroyMesh() = 0;

  virtual bool CreateTexture() = 0;
  virtual bool DestroyTexture() = 0;

  virtual bool CreateShaderModule(ShaderHandle shaderHandle) = 0;
  virtual bool DestroyShaderModule() = 0;

  virtual bool CreatePipeline() = 0;
  virtual bool DestroyPipeline() = 0;

  virtual bool CreateComputeEffect() = 0;
  virtual bool DestoryComputeEffect() = 0;
};