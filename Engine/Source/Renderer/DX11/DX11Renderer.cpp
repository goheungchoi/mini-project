#include "DX11Renderer.h"
#include "Internal/DebugLayer.h"
#include "Internal/Device.h"
#include "Internal/RenderFrameworks/Pipeline.h"
#include "Internal/Resources/Buffer.h"
#include "Internal/SwapChain.h"
#include "ResourceManager/ResourceManager.h"

DX11Renderer::~DX11Renderer() {}
bool DX11Renderer::Init_Win32(int width, int height, void* hInstance,
                              void* hwnd)
{
  HWND* pHwnd = reinterpret_cast<HWND*>(hwnd);
  _device = new Device;
  _swapChain = new SwapChain;
  _debugLayer = new DebugLayer;
  _device->Init();
  _swapChain->Init(pHwnd, _device->GetDevice(), width, height);
#ifdef _DEBUG
  _debugLayer->Init(_device->GetDevice());
  // Enable breaking on errors
  _debugLayer->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

  // Optionally, you can also enable breaking on warnings or other severities
  //_debugLayer->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);

  // After debugging, you can disable specific breakpoints
  //_debugLayer->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, false);
#endif // _DEBUG

  return true;
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

bool DX11Renderer::CreateMesh(MeshHandle handle)
{
  auto iter = _meshMap.find(handle);

  if (iter==_meshMap.end())
  {
    MeshBuffer* meshBuffer = new MeshBuffer;

    MeshData meshData = AccessMeshData(handle);

    uint32_t size = sizeof(Vertex) * meshData.vertices.size();
    meshBuffer->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);

    size = sizeof(UINT) * meshData.indices.size();
    meshBuffer->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);

    meshBuffer->nIndices = meshData.indices.size();
    meshBuffer->stride = 0;
    meshBuffer->offset = sizeof(Vertex);
    _meshMap.insert({handle, meshBuffer});
  }
  
  return true;
}

bool DX11Renderer::DestroyMesh()
{
  // delete mesh
  for (auto& mesh : _meshMap)
  {
    delete mesh.second;
  }
  return false;
}

bool DX11Renderer::CreateTexture()
{

  delete _device;
  delete _swapChain;
  delete _debugLayer;

  return true;
}

bool DX11Renderer::DestroyTexture()
{
  return false;
}

bool DX11Renderer::CreateShaderModule(ShaderHandle shaderHandle)
{
  ShaderData data = AccessShaderData(shaderHandle);

  return false;
}

bool DX11Renderer::DestroyShaderModule()
{
  return false;
}

bool DX11Renderer::CreatePipeline()
{

  return true;
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
