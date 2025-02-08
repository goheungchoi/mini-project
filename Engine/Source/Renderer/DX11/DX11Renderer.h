#pragma once
#include "Renderer/IRenderer.h"
struct ResourceStorage;
#ifdef _DEBUG
class DebugLayer;
#endif // _DEBUG
class Device;
class SwapChain;
class PipeLine;
class RenderPassManager;
class SkyBox;
class D2DRenderer;
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
                  DirectionalLight mainLight) override;
  void BeginDraw(MeshHandle handle, Matrix world) override;
  void DrawMesh(MeshHandle handle, Matrix world, RenderTypeFlags type,
                Color outlineColor = Color(1.f, 1.f, 0.f, 1.f),
                vector<DirectX::XMMATRIX> boneTransforms =
                    vector<DirectX::XMMATRIX>()) override;
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
  bool CreateComputeEffect() override;
  bool DestoryComputeEffect() override;
  void CreateSkyBox(LPCSTR envPath, LPCSTR specularBRDFPath,
                    LPCSTR diffuseIrrPath, LPCSTR specularIBLPath) override;

  void CreateBillboard(Billboard*& billboard) override;
  void DrawDebugSphere(Matrix world, Color color) override;
  void DrawDebugBox(Matrix world, Color color) override;
  void DrawDebugCylinder(Matrix world, Color color) override;
  void DrawBillBoard(Billboard* billboard) override;
  void CreateTrail(Trail*& trail) override;
  void DrawTrail(Trail* trail) override;

public:
  // Render ImGui graphical interface.
  void BeginImGuiDraw();
  void DrawImGui();

  // D2D Renderer
  void CreateSprite(LPCSTR path, Vector2 pos) override;
  void DrawTexts(const wchar_t* format, Vector4 rect, Color color,
                 float opacity, const TextFormatInfo& textFormatInfo) override;

  void DrawRectangle(Color color, Vector4 rect, float stroke = 1.0f,
                     float opacity = 1.0f) override;

  void FillRectangle(Color color, Vector4 rect, float opacity = 1.0f) override;

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

  D2DRenderer* _d2dRenderer = nullptr;
};