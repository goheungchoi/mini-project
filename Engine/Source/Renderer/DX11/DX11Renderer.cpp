#include "DX11Renderer.h"
#include "Internal/Device.h"
DX11Renderer::~DX11Renderer() {}
bool DX11Renderer::Init_Win32(UINT width, UINT height, void* hInstance,
                              void* hwnd)
{
  HWND* pHwnd = reinterpret_cast<HWND*>(hwnd);
  pDevice->CreateDevice(pHwnd,width,height);
  return false;
}

bool DX11Renderer::Cleanup()
{
  return false;
}

void DX11Renderer::ResizeScreen(unsigned int width, unsigned int height) {}

void DX11Renderer::BeginFrame() {}

void DX11Renderer::BeginDraw() {}

void DX11Renderer::EndDraw() {}

void DX11Renderer::EndFrame() {}

void DX11Renderer::BindPipeline() {}

void DX11Renderer::BindResource() {}

bool DX11Renderer::CreateMesh()
{
  return false;
}

bool DX11Renderer::DestroyMesh()
{
  return false;
}

bool DX11Renderer::CreateTexture()
{
  return false;
}

bool DX11Renderer::DestroyTexture()
{
  return false;
}

bool DX11Renderer::CreateShaderModule()
{
  return false;
}

bool DX11Renderer::DestroyShaderModule()
{
  return false;
}

bool DX11Renderer::CreatePipeline()
{
  return false;
}

bool DX11Renderer::DestroyPipeline()
{
  return false;
}

bool DX11Renderer::CreateComputeEffect()
{
  return false;
}

bool DX11Renderer::DestoryComputeEffect()
{
  return false;
}
