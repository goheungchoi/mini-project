#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Objects/Swapchain.h"
#include "../RenderFrameworks/DeferredPass.h"
#include "../RenderFrameworks/Shader.h"
#include "../RenderFrameworks/ShadowPass.h"
#include "../Resources/ConstantBuffer.h"
#include "../Resources/GeometryPrimitive.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/Sampler.h"
#include "../Resources/SkyBox.h"
#include "Core/Common.h"

namespace Renderer
{
struct Camera
{
  Vector4 eye;
  Matrix view;
  Matrix projection;
};
} // namespace Renderer
using namespace std;
class RenderPassManager
{
private:
  // mesh
  // 0 : single sided 1 : double sided
  vector<map<pair<float, int>, MeshBuffer*, greater<std::pair<float, int>>>>
      _transparentMeshes;
  // 0 : single sided 1 : double sided
  vector<vector<MeshBuffer*>> _opaqueMesh;
  vector<vector<MeshBuffer*>> _shadowMesh;
  SkyBox* _skyBox = nullptr;
  GeometryPrimitive* _geometry = nullptr;
  // Constant Buffer
  MeshConstantBuffer* _CB;
  FrameConstantBuffer* _frameCB;
  // pass
  ShadowPass* _shadow;
  DefferedPass* _deffered;
  // sampler
  vector<Sampler*> _samplers;
  // shader
  unordered_map<std::string, VertexShader*> _vShaders;
  unordered_map<std::string, PixelShader*> _pShaders;

private:
  // renderer에서 생성한 deive 참조.
  Device* _device;
  PipeLine* _pso = nullptr;
  ShaderCompiler* _compiler = nullptr;
  Renderer::Camera _camera;
  DirectionalLight _mainLight;
  int max = std::numeric_limits<int>::max();

public:
  RenderPassManager(Device* device, SwapChain* swapchain, int width, int height)
      : _device{device}
  {
    _pso = new PipeLine(_device, swapchain, width, height);
    _device->GetImmContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _frameCB = new FrameConstantBuffer(_device);
    _CB = new MeshConstantBuffer(_device);
    _compiler = new ShaderCompiler(_device);
    _shadow = new ShadowPass(_device, 8192, 8192);
    _deffered = new DefferedPass(width, height, _device);
    _skyBox = new SkyBox(_device);
    _geometry = new GeometryPrimitive(_device);
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->VSSetConstantBuffers(
        1, 1,
        _CB->_constantBuffers[static_cast<UINT>(MeshCBType::World)]
            .GetAddressOf());
    dc->PSSetConstantBuffers(
        2, 1,
        _CB->_constantBuffers[static_cast<UINT>(MeshCBType::PixelData)]
            .GetAddressOf());
    dc->VSSetConstantBuffers(0, 1, _frameCB->_constantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(0, 1, _frameCB->_constantBuffer.GetAddressOf());
    _transparentMeshes.resize(2);
    _opaqueMesh.resize(2);
    _shadowMesh.resize(2);
  }
  ~RenderPassManager()
  {
    std::ranges::for_each(_samplers, [](Sampler* sam) { SAFE_RELEASE(sam); });
    for (auto& [key, value] : _vShaders)
    {
      SAFE_RELEASE(value);
    }
    for (auto& [key, value] : _pShaders)
    {
      SAFE_RELEASE(value);
    }
    SAFE_RELEASE(_compiler);
    SAFE_RELEASE(_CB);
    SAFE_RELEASE(_frameCB);
    SAFE_RELEASE(_deffered);
    SAFE_RELEASE(_shadow);
    SAFE_RELEASE(_pso);
    SAFE_RELEASE(_skyBox);
    SAFE_RELEASE(_geometry);
  }

public:
  void SetCamera(Vector4 eye, Matrix view, Matrix projection)
  {
    _camera.view = view;
    _camera.projection = projection;
    _camera.eye = eye;
  }
  void SetMainLightDir(DirectionalLight mainLight)
  {
    _mainLight = mainLight;
  }
  void ClassifyPass(MeshBuffer* buff)
  {
    if (buff->flags & RenderPassType::kOpaquePass)
    {
      if (!buff->material->doubleSided)
        _opaqueMesh[0].push_back(buff);
      else
        _opaqueMesh[1].push_back(buff);
    }
    if (buff->flags & RenderPassType::kTransparentPass)
    {
      // 뷰 공간의 Z값 계산
      Vector3 worldPos3 = buff->world.Translation(); // 월드 공간에서의 위치
      Vector4 worldPos = {worldPos3.x, worldPos3.y, worldPos3.z,
                          1.f}; // 월드 공간에서의 위치
      Vector4 viewPos =
          Vector4::Transform(worldPos, _camera.view); // 뷰 공간으로 변환
      float viewZ = viewPos.z;                        // 뷰 공간 Z값 추출
      if (!buff->material->doubleSided)
        _transparentMeshes[0].insert({{viewZ, max}, buff});
      else
        _transparentMeshes[1].insert({{viewZ, max}, buff});

      max--;
    }

    if (buff->flags & RenderPassType::kShadowPass)
    {
      if (!buff->material->doubleSided)
        _shadowMesh[0].push_back(buff);
      else
        _shadowMesh[1].push_back(buff);
    }
  }
  void ProcessPass()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    _pso->ClearBackBufferDSV();
    _pso->SetMainRS();
    // Shadow pass
    this->DrawShadow(dc);
    //--------------------------------------------------------------------------------------//
    // Deferred pass
    this->DrawDeffered(dc);

    //--------------------------------------------------------------------------------------//
    // Transparent pass -> Forward renderin
    this->DrawTransparency(dc);

//--------------------------------------------------------------------------------------//
// wireFrame pass -> Forward rendering
#ifdef _DEBUG
    this->DrawGeometryPrimitveWireFrame(dc);
#endif

    // 처리후 클리어
    this->Clear();

    // max값 초기화
    max = std::numeric_limits<int>::max();
  }
  /**
   * @brief call per frame, update frame constantBuffer
   */
  void FrameSet()
  {
    Constant::Frame frame = {
        .mainDirectionalLight = _mainLight,
        .cameraPosition = _camera.eye,
        .view = _camera.view.Transpose(),
        .projection = _camera.projection.Transpose(),
    };
    Matrix IView = _camera.view;
    Matrix IProj = _camera.projection;
    IView = DirectX::XMMatrixInverse(nullptr, IView);
    IProj = DirectX::XMMatrixInverse(nullptr, IProj);
    frame.inverseView = DirectX::XMMatrixTranspose(IView);
    frame.inverseProjection = DirectX::XMMatrixTranspose(IProj);
    _shadow->CalculateMatrix(_camera.view, _camera.eye, _mainLight.direction);
    frame.shadwoView = _shadow->View.Transpose();
    frame.shadowProjection = _shadow->Projection.Transpose();
    _frameCB->UpdateContantBuffer(frame);
  }
  void SetSkyBox(LPCSTR envPath, LPCSTR specularBRDFPath, LPCSTR diffuseIrrPath,
                 LPCSTR specularIBLPath)
  {
    _skyBox->Init();
    _skyBox->SetTexture(envPath, specularBRDFPath, diffuseIrrPath,
                        specularIBLPath);
  }
  ID3D11RenderTargetView* GetBackBuffer()
  {
    return _pso->_backBuffer->mainRTV.Get();
  }
  void CreateMainShader()
  {
    // vs
    std::vector<D3D_SHADER_MACRO> macros;
    macros = {{nullptr, nullptr}};
    _vShaders.insert(
        {"Default", _compiler->CompileVertexShader(macros, "vs_main")});
    macros.clear();
    macros = {{"Skinning", "1"}, {nullptr, nullptr}};
    _vShaders.insert(
        {"Skinning", _compiler->CompileVertexShader(macros, "vs_main")});
    macros.clear();
    macros = {{"Quad", "1"}, {nullptr, nullptr}};
    _vShaders.insert(
        {"Quad", _compiler->CompileVertexShader(macros, "quad_vs_main")});
    macros.clear();
    macros = {{"SkyBox", "1"}, {nullptr, nullptr}};
    _vShaders.insert(
        {"SkyBox", _compiler->CompileVertexShader(macros, "skybox_vs_main")});
    macros.clear();
    macros = {{"Shadow", "1"}, {nullptr, nullptr}};
    _vShaders.insert(
        {"Shadow", _compiler->CompileVertexShader(macros, "shadow_vs_main")});
    // ps
    macros = {{"Deffered", "1"}, {nullptr, nullptr}};
    _pShaders.insert(
        {"Deffered", _compiler->CompilePixelShader(macros, "ps_main")});
    macros.clear();
    macros = {{"Transparency", "1"}, {nullptr, nullptr}};
    _pShaders.insert(
        {"Transparency", _compiler->CompilePixelShader(macros, "ps_main")});
    macros.clear();
    macros = {{"Quad", "1"}, {nullptr, nullptr}};
    _pShaders.insert(
        {"Quad", _compiler->CompilePixelShader(macros, "quad_ps_main")});
    macros.clear();
    macros = {{"SkyBox", "1"}, {nullptr, nullptr}};
    _pShaders.insert(
        {"SkyBox", _compiler->CompilePixelShader(macros, "skybox_ps_main")});
#ifdef _DEBUG
    macros.clear();
    macros = {{"WireFrame", "1"}, {nullptr, nullptr}};
    _pShaders.insert({"WireFrame", _compiler->CompilePixelShader(
                                       macros, "wire_frame_ps_main")});
#endif
  }
  void CreateSamplers()
  {
    D3D11_SAMPLER_DESC sampleDesc = {};
    // linear
    ZeroMemory(&sampleDesc, sizeof(D3D11_SAMPLER_DESC));
    sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::LINEAR_WRAP);
    // iblspecularBRDF
    // clamp
    ZeroMemory(&sampleDesc, sizeof(D3D11_SAMPLER_DESC));
    sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.MaxAnisotropy = 1;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::LINEAR_CLAMP);
    // default
    // anisotropy
    ZeroMemory(&sampleDesc, sizeof(D3D11_SAMPLER_DESC));
    sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::ANIMSOROPIC);
    // Comparison
    ZeroMemory(&sampleDesc, sizeof(D3D11_SAMPLER_DESC));
    sampleDesc = {};
    sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampleDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    MakeSampler(sampleDesc, SamplerType::COMPARISON);

    std::vector<ID3D11SamplerState*> samplers;
    std::ranges::for_each(_samplers, [&samplers](Sampler* sam) {
      samplers.push_back(sam->state.Get());
    });
    _device->GetImmContext()->PSSetSamplers(
        0, static_cast<UINT>(samplers.size()), samplers.data());
  }
#ifdef _DEBUG
  void UpdateVariable()
  {
    // SWTODO : _shadow ->update 변수 처리하기.
    _shadow->UpdateVarialbe();
  }

private:
  vector<pair<Matrix, Color>> _spheres;
  vector<pair<Matrix, Color>> _boxes;
  vector<pair<Matrix, Color>> _cylinders;

public:
  void ClassifyGeometryPrimitive(Geometry::Type type, Matrix world, Color color)
  {
    switch (type)
    {
    case Geometry::Type::Box:
      _boxes.push_back({world, color});
      break;
    case Geometry::Type::Sphere:
      _spheres.push_back({world, color});
      break;
    case Geometry::Type::Cylinder:
      _cylinders.push_back({world, color});
      break;
    case Geometry::Type::End:
      break;
    default:
      break;
    }
  }
#endif

private:
  void MakeSampler(D3D11_SAMPLER_DESC desc, SamplerType type)
  {
    Sampler* sam = new Sampler;
    sam->type = type;
    _device->GetDevice()->CreateSamplerState(&desc, sam->state.GetAddressOf());
    _samplers.push_back(sam);
  }
#ifdef _DEBUG
  void DrawGeometryPrimitveWireFrame(ID3D11DeviceContext* dc)
  {
    _pso->SetWireFrame();
    dc->PSSetShader(_pShaders["WireFrame"]->shader.Get(), nullptr, 0);
    // box
    dc->IASetVertexBuffers(
        0, 1, _geometry->box.buffer->vertexBuffer.GetAddressOf(),
        &(_geometry->box.buffer->stride), &(_geometry->box.buffer->offset));
    dc->IASetIndexBuffer(_geometry->box.buffer->indexBuffer.Get(),
                         DXGI_FORMAT_R32_UINT, 0);
    std::ranges::for_each(_boxes, [this, dc](const auto& pair) {
      const auto& [first, second] = pair;
      Constant::World world = {first.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {.color = second};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      dc->DrawIndexed(_geometry->box.buffer->nIndices, 0, 0);
    });
    // sphere
    dc->IASetVertexBuffers(
        0, 1, _geometry->sphere.buffer->vertexBuffer.GetAddressOf(),
        &(_geometry->sphere.buffer->stride),
        &(_geometry->sphere.buffer->offset));
    dc->IASetIndexBuffer(_geometry->sphere.buffer->indexBuffer.Get(),
                         DXGI_FORMAT_R32_UINT, 0);
    std::ranges::for_each(_spheres, [this, dc](const auto& pair) {
      const auto& [first, second] = pair;
      Constant::World world = {first.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {.color = second};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      dc->DrawIndexed(_geometry->sphere.buffer->nIndices, 0, 0);
    });
    // cylinder
    dc->IASetVertexBuffers(
        0, 1, _geometry->cylinder.buffer->vertexBuffer.GetAddressOf(),
        &(_geometry->cylinder.buffer->stride),
        &(_geometry->cylinder.buffer->offset));
    dc->IASetIndexBuffer(_geometry->cylinder.buffer->indexBuffer.Get(),
                         DXGI_FORMAT_R32_UINT, 0);
    std::ranges::for_each(_cylinders, [this, dc](const auto& pair) {
      const auto& [first, second] = pair;
      Constant::World world = {first.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {.color = second};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      dc->DrawIndexed(_geometry->cylinder.buffer->nIndices, 0, 0);
    });
  }
#endif

  void DrawShadow(ID3D11DeviceContext* dc)
  {
    _shadow->Prepare();
    dc->IASetInputLayout(_vShaders["Shadow"]->layout.Get());
    dc->VSSetShader(_vShaders["Shadow"]->shader.Get(), nullptr, 0);
    dc->PSSetShader(nullptr, nullptr, 0);

    std::ranges::for_each(_shadowMesh[0], [this, dc](MeshBuffer* buffer) {
      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    std::ranges::for_each(_shadowMesh[1], [this, dc](MeshBuffer* buffer) {
      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
  }

  void DrawDeffered(ID3D11DeviceContext* dc)
  {
    // skybox
    _pso->SetBlendOnEnable(false);
    _pso->SetMainViewPort();
    _pso->SetMainRS();
    _deffered->Prepare(_pso->_backBuffer);
    dc->IASetInputLayout(_vShaders["SkyBox"]->layout.Get());
    dc->VSSetShader(_vShaders["SkyBox"]->shader.Get(), nullptr, 0);
    dc->PSSetShader(_pShaders["SkyBox"]->shader.Get(), nullptr, 0);
    Constant::World world = {_skyBox->GetMesh()->world.Transpose()};
    _CB->UpdateContantBuffer(world, MeshCBType::World);
    _skyBox->Render();
    // Deferred meshes
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    dc->PSSetShader(_pShaders["Deffered"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_opaqueMesh[0], [this, dc](MeshBuffer* buffer) {
      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {.alphaCutoff = buffer->material->alphaCutoff};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    std::ranges::for_each(_opaqueMesh[1], [this, dc](MeshBuffer* buffer) {
      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {.alphaCutoff = buffer->material->alphaCutoff};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
    _pso->ClearBackBufferRTV();
    _pso->TurnZBufferOff();
    dc->IASetInputLayout(_vShaders["Quad"]->layout.Get());
    dc->VSSetShader(_vShaders["Quad"]->shader.Get(), nullptr, 0);
    dc->PSSetShader(_pShaders["Quad"]->shader.Get(), nullptr, 0);
    _pso->SetBackBuffer();
    _shadow->SetDepthSRV();
    _deffered->QuadDraw();
    _deffered->ClearRenderTargets();
  }

  void DrawTransparency(ID3D11DeviceContext* dc)
  {
    _pso->SetBlendOnEnable(true);
    _pso->SetMainRS();
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    dc->PSSetShader(_pShaders["Transparency"]->shader.Get(), nullptr, 0);
    _pso->TurnZBufferOn();
    std::ranges::for_each(_transparentMeshes[0], [this, dc](const auto& pair) {
      const auto& [z, buffer] = pair;

      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    std::ranges::for_each(_transparentMeshes[0], [this, dc](const auto& pair) {
      const auto& [z, buffer] = pair;

      dc->IASetVertexBuffers(0, 1, buffer->vertexBuffer.GetAddressOf(),
                             &(buffer->stride), &(buffer->offset));
      dc->IASetIndexBuffer(buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      Constant::World world = {buffer->world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(buffer->nIndices, 0, 0);
    });
  }

  void Clear()
  {
    std::ranges::for_each(_opaqueMesh,
                          [](std::vector<MeshBuffer*>& vec) { vec.clear(); });
    std::ranges::for_each(
        _transparentMeshes,
        [](map<pair<float, int>, MeshBuffer*, greater<std::pair<float, int>>>&
               map) { map.clear(); });
    std::ranges::for_each(_shadowMesh,
                          [](std::vector<MeshBuffer*>& vec) { vec.clear(); });
#ifdef _DEBUG
    _spheres.clear();
    _boxes.clear();
    _cylinders.clear();
#endif
  }
};
