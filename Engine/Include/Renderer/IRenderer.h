#pragma once
#include "Core/Handle/ResourceHandle.h"
#include "Core/Common.h"
#include "Core/Types/LightTypes.h"
using namespace std;
/**
 * @brief 
 * 1. call AddRenderPass() per mesh at initialize
 * 2. call BeginFrame() per frame 
 * 3. call BeinDraw() per mesh every frame
 * 4. call DrawMesh() per mesh every frame
 * 5. call EndFrame() per frame
 */
class IRenderer
{
public:
  virtual bool Init_Win32(int width, int height, void* hInstance,
                          void* hwnd) = 0;

  virtual bool Cleanup() = 0;

  virtual void ResizeScreen(unsigned int width, unsigned int height) = 0;
  /**
   * @brief call per frame
   * @param cameraPos : camera position
   * @param view : camera view matrix
   * @param projection : camera projection matrix
   * @param mainLightDir : main light direction
   */
  virtual void BeginFrame(Vector4 cameraPos, Matrix view, Matrix projection,
                          DirectionalLight mainLight) = 0;
  /**
   * @brief call per mesh
   * @param handle : mesh handle owned by mesh
   * @param world : world matrix of mesh
   */
  virtual void BeginDraw(MeshHandle handle, Matrix world) = 0;
  /**
   * @brief call per mesh
   * @param handle : mesh handle owned by mesh
   */
  virtual void DrawMesh(MeshHandle handle, Matrix world,
                        vector<DirectX::XMMATRIX> boneTransforms =
                                               vector<DirectX::XMMATRIX>()) = 0;
  /**
   * @brief still working on it....
   */
  virtual void EndDraw() = 0;
  /**
   * @brief call per frame
   */
  virtual void EndFrame() = 0;

  /*
   * @brief 
     1. you can add more than one call before begin draw
     2. Runs only the type of render path you added, for this mesh
     3. don't call every loop
   * Param1 : own meshHandle
   */
  virtual void AddShadow(MeshHandle handle) = 0;
  virtual void DeleteShadow(MeshHandle handle) = 0;
  ///*
  // * @brief
  //   1. you can delete meshes render pass type.
  // * Param1 : own meshHandle
  // */
  #ifdef _DEBUG
  /**
   * @brief draw Sphere wireFrame, it's only work on debug
   * @param world world Matirx
   * @param color color
   */
  virtual void DrawDebugSphere(Matrix world, Color color) = 0;
  /**
   * @brief draw Sphere wireFrame, it's only work on debug
   * @param world world Matirx
   * @param color color
   */
  virtual void DrawDebugBox(Matrix world, Color color) = 0;
  /**
   * @brief draw Sphere wireFrame, it's only work on debug
   * @param world world Matirx
   * @param color color
   */
  virtual void DrawDebugCylinder(Matrix world, Color color) = 0;
  #endif

  virtual void AddOutLine(MeshHandle handle) = 0;
  virtual void DeleteOutLine(MeshHandle handle) = 0;
  

  virtual void BindPipeline() = 0;

  virtual void BindResource() = 0;

  // 렌더러 안에서 D3D11 리소스 생성과 파괴
  virtual bool CreateMesh(MeshHandle handle) = 0;
  virtual bool DestroyMesh() = 0;

  virtual bool CreateTexture() = 0;
  virtual bool DestroyTexture() = 0;

  virtual bool CreateShaderModule(ShaderHandle shaderHandle) = 0;
  virtual bool DestroyShaderModule() = 0;

  virtual bool CreateComputeEffect() = 0;
  virtual bool DestoryComputeEffect() = 0;

  virtual void CreateSkyBox(LPCSTR envPath, LPCSTR specularBRDFPath,
                            LPCSTR diffuseIrrPath, LPCSTR specularIBLPath) = 0;

  
  // D2D Renderer
  virtual void CreateSprite(LPCSTR path, Vector2 pos) = 0;

  virtual void AddText(const wchar_t* format, Vector4 rect,
                        const std::wstring& fontName = L"Agency FB",
                        Color color = Color(1.0f, 0.0f, 1.0f, 1.0f)) = 0;

};