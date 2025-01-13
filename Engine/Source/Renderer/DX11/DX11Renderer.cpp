#include "DX11Renderer.h"
#include "Internal/DebugLayer.h"
#include "Internal/Device.h"
#include "Internal/RenderFrameworks/RenderPass.h"
#include "Internal/ResourceStorage.h"
#include "Internal/Resources/Material.h"
#include "Internal/Resources/PipeLineState.h"
#include "Internal/SwapChain.h"

DX11Renderer::~DX11Renderer() {}
bool DX11Renderer::Init_Win32(int width, int height, void* hInstance,
                              void* hwnd)
{
  HWND* pHwnd = reinterpret_cast<HWND*>(hwnd);
  _device = new Device;
  _swapChain = new SwapChain;
  _debugLayer = new DebugLayer;
  _storage = new ResourceStorage;
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
  _passMgr = new RenderPassManager(_device, _swapChain, width, height);
  _passMgr->CreateSamplers();
  _passMgr->CreateMainShader();

  return true;
}

// 종료 직전에 콜해주기
bool DX11Renderer::Cleanup()
{
  DestroyMesh();
  DestroyPipeline();
  DestroyTexture();
  DestroyShaderModule();
  SAFE_RELEASE(_passMgr);
  SAFE_RELEASE(_device);
  SAFE_RELEASE(_debugLayer);
  SAFE_RELEASE(_swapChain);
  SAFE_RELEASE(_storage);
  return false;
}

void DX11Renderer::ResizeScreen(unsigned int width, unsigned int height) {}

void DX11Renderer::BeginFrame(Vector4 cameraPos, Matrix view, Matrix projection,
                              Light::DirectionalLight mainLight)
{
  _passMgr->SetCamera(cameraPos, view, projection);
  _passMgr->SetMainLightDir(mainLight);
  _passMgr->ClearBackBuffer();
}

void DX11Renderer::BeginDraw(MeshHandle handle, Matrix world)
{
  auto buffer = _storage->meshMap.find(handle);
  if (buffer == _storage->meshMap.end())
  {
    throw std::exception("buffer not registered");
  }
  buffer->second->world = world;
}

void DX11Renderer::DrawMesh(MeshHandle handle)
{
  auto buffer = _storage->meshMap.find(handle);
  if (buffer == _storage->meshMap.end())
  {
    throw std::exception("buffer not registered");
  }
  _passMgr->ClassfyPass(buffer->second);
}

void DX11Renderer::EndDraw() {}

void DX11Renderer::EndFrame()
{
  _passMgr->FrameSet();
  _passMgr->ProcessPass();

  _swapChain->GetSwapChain()->Present(0, 0);
}

void DX11Renderer::AddRenderPass(MeshHandle handle, RenderPassType type)
{
  auto buffer = _storage->meshMap.find(handle);
  if (buffer != _storage->meshMap.end())
  {
    if (buffer->second->flags & type)
    {
      return;
    }
    buffer->second->flags |= type;
  }
}

void DX11Renderer::DeleteRenderPass(MeshHandle handle, RenderPassType type)
{
  auto buffer = _storage->meshMap.find(handle);
  if (buffer != _storage->meshMap.end())
  {
    if (!(buffer->second->flags & type))
    {
      return;
    }
    buffer->second->flags &= ~type;
  }
}

void DX11Renderer::BindPipeline() {}

void DX11Renderer::BindResource() {}

bool DX11Renderer::CreateMesh(MeshHandle handle)
{
  auto iter = _storage->meshMap.find(handle);

  if (iter == _storage->meshMap.end())
  {
    MeshBuffer* meshBuffer = new MeshBuffer;

    MeshData meshData = AccessMeshData(handle);
    // material
    {
      meshBuffer->material = new Material;
      MaterialData matData = AccessMaterialData(meshData.material);
      meshBuffer->material->CreateMaterial(_device, matData);
    }
    // SWTODO : 나중에 skeletal이냐 static이냐 구분해야함.??
    uint32_t size = sizeof(Vertex) * meshData.vertices.size();
    meshBuffer->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);

    size = sizeof(UINT) * meshData.indices.size();
    meshBuffer->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);

    meshBuffer->nIndices = meshData.indices.size();
    meshBuffer->stride = sizeof(Vertex);
    meshBuffer->offset = 0;
    _storage->meshMap.insert({handle, meshBuffer});
  }

  return true;
}

bool DX11Renderer::DestroyMesh()
{
  // delete mesh
  for (auto& mesh : _storage->meshMap)
  {
    delete mesh.second;
  }
  _storage->meshMap.clear();
  return false;
}

bool DX11Renderer::CreateTexture()
{

  return true;
}

bool DX11Renderer::DestroyTexture()
{
  return false;
}

bool DX11Renderer::CreateShaderModule(ShaderHandle shaderHandle)
{
  ShaderData data = AccessShaderData(shaderHandle);
  if (data.data.empty())
  {
    // SWTODO : 에러처리.
    return false;
  }

  switch (data.type)
  {
  case ShaderType::kVertex: {
    auto shader = _storage->vertexShaderMap.find(shaderHandle);
    if (shader == _storage->vertexShaderMap.end())
    {
      ComPtr<ID3D11VertexShader> vs;
      HR_T(_device->GetDevice()->CreateVertexShader(
          data.data.data(), data.data.size(), nullptr, &vs));
      _storage->vertexShaderMap[shaderHandle].shader = vs;
      std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
      CreateInputLayoutDesc(descs, data.data, data.data.size());
      _storage->vertexShaderMap[shaderHandle].layout =
          _device->CreateInputLayout(descs, data.data);
      std::ranges::for_each(descs, [](D3D11_INPUT_ELEMENT_DESC& desc) {
        SAFE_RELEASE(desc.SemanticName);
      });
    }
    break;
  }
  case ShaderType::kPixel: {
    auto shader = _storage->pixelShaderMap.find(shaderHandle);
    if (shader == _storage->pixelShaderMap.end())
    {
      ComPtr<ID3D11PixelShader> ps;
      HR_T(_device->GetDevice()->CreatePixelShader(
          data.data.data(), data.data.size(), nullptr, &ps));
      _storage->pixelShaderMap[shaderHandle] = ps;
    }
    break;
  }
  }

  return true;
}

bool DX11Renderer::DestroyShaderModule()
{
  _storage->vertexShaderMap.clear();
  _storage->pixelShaderMap.clear();
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

void DX11Renderer::CreateEngineShader()
{
  _passMgr->CreateMainShader();
}
