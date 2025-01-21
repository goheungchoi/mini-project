#pragma once
#include "Core/Handle/ResourceHandle.h"
#include "DX11/Internal/Types.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
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
                          Light::DirectionalLight mainLight) = 0;
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
  virtual void DrawMesh(MeshHandle handle) = 0;
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
  ///*
  // * @brief
  //   1. you can delete meshes render pass type.
  // * Param1 : own meshHandle
  // */
  #ifdef _DEBUG
  virtual void DrawDebugSphere() = 0;
  virtual void DrawDebugBox() = 0;
  virtual void DrawDebugCylinder() = 0;
  #endif
  virtual void DeleteShadow(MeshHandle handle) = 0;

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

  virtual void CreateSkyBox(LPCSTR envPath, LPCSTR specularBRDFPath,
                            LPCSTR diffuseIrrPath, LPCSTR specularIBLPath) = 0;
};