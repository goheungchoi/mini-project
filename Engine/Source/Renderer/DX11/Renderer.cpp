#include "Renderer.h"

bool Renderer::Init_Win32(void *hInstance, void *hwnd)
{
  return false;
}

bool Renderer::Cleanup()
{
  return false;
}

void Renderer::ResizeScreen(unsigned int width, unsigned int height)
{
}

void Renderer::BeginFrame()
{
}

void Renderer::BeginDraw()
{
}

void Renderer::EndDraw()
{
}

void Renderer::EndFrame()
{
}

void Renderer::BindPipeline()
{
}

void Renderer::BindResource()
{
}

bool Renderer::CreateMesh()
{
  return false;
}

bool Renderer::DestroyMesh()
{
  return false;
}

bool Renderer::CreateTexture()
{
  return false;
}

bool Renderer::DestroyTexture()
{
  return false;
}

bool Renderer::CreateShaderModule()
{
  return false;
}

bool Renderer::DestroyShaderModule()
{
  return false;
}

bool Renderer::CreatePipeline()
{
  return false;
}

bool Renderer::DestroyPipeline()
{
  return false;
}

bool Renderer::CreateComputeEffect()
{
  return false;
}

bool Renderer::DestoryComputeEffect()
{
  return false;
}
