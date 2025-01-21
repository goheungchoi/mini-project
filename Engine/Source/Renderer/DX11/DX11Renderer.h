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
class SkyBox;
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
  void BeginFrame(Vector4 cameraPos, Matrix view, Matrix projection,
                  Light::DirectionalLight mainLight) override;
  void BeginDraw(MeshHandle handle, Matrix world) override;
  void DrawMesh(MeshHandle handle) override;
  void EndDraw() override;
  void EndFrame() override;
  void AddShadow(MeshHandle handle) override;
  void DeleteShadow(MeshHandle handle) override;
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
  void CreateSkyBox(LPCSTR envPath, LPCSTR specularBRDFPath,
                    LPCSTR diffuseIrrPath, LPCSTR specularIBLPath) override;
  #ifdef _DEBUG
  void DrawDebugSphere()override;
  void DrawDebugBox()override;
  void DrawDebugCylinder() override;
  #endif
public:
	// Render ImGui graphical interface.
  void BeginImGuiDraw();
  void DrawImGui();

private:
  void CreateEngineShader();

	// Initialize ImGui
  void InitImGui();

private:
  Device* _device = nullptr;
  SwapChain* _swapChain = nullptr;
#ifdef _DEBUG
  DebugLayer* _debugLayer = nullptr;
#endif // _DEBUG
  ResourceStorage* _storage = nullptr;
  RenderPassManager* _passMgr = nullptr;
};