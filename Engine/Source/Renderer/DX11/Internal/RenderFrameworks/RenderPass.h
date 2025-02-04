#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Objects/Swapchain.h"
#include "../RenderFrameworks/DeferredPass.h"
#include "../RenderFrameworks/PostProcess/OutLinePass.h"
#include "../RenderFrameworks/PostProcess/SSAOPass.h"
#include "../RenderFrameworks/Shader.h"
#include "../RenderFrameworks/ShadowPass.h"
#include "../Resources/BillboardQuad.h"
#include "../Resources/ConstantBuffer.h"
#include "../Resources/GeometryPrimitive.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/Sampler.h"
#include "../Resources/SkyBox.h"
#include "Core/Common.h"
#include "Core/Types/RenderType.h"
#include "Core/Math/MathUtils.h"
using namespace RenderMesh;
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
  vector<map<pair<float, int>, StaticMesh, greater<std::pair<float, int>>>>
      _staticTransMeshes;
  vector<map<pair<float, int>, SkelMesh, greater<std::pair<float, int>>>>
      _skelTransMeshes;
  // 0 : single sided 1 : double sided
  vector<vector<StaticMesh>> _staticOpaqueMesh;
  vector<vector<SkelMesh>> _skelOpaqueMesh;
  // 0 : single sided 1 : double sided
  vector<vector<StaticMesh>> _staticShadowMesh;
  vector<vector<SkelMesh>> _skelShadowMesh;
  SkyBox* _skyBox = nullptr;
  GeometryPrimitive* _geometry = nullptr;
  // Constant Buffer
  MeshConstantBuffer* _CB;
  FrameConstantBuffer* _frameCB;
  // sampler
  vector<Sampler*> _samplers;
  // shader
  unordered_map<std::string, VertexShader*> _vShaders;
  unordered_map<std::string, PixelShader*> _pShaders;
  // Billboard
  vector<BillboardQuad*> _billboards;
  // pass
  ShadowPass* _shadow = nullptr;
  DefferedPass* _deffered = nullptr;
  OutLinePass* _outLine = nullptr;
  SSAOPass* _ssao = nullptr;

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
    _outLine = new OutLinePass(_CB);
    _ssao = new SSAOPass(_device);
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->VSSetConstantBuffers(
        2, 1,
        _CB->_constantBuffers[static_cast<UINT>(MeshCBType::World)]
            .GetAddressOf());
    dc->PSSetConstantBuffers(
        3, 1,
        _CB->_constantBuffers[static_cast<UINT>(MeshCBType::PixelData)]
            .GetAddressOf());
    dc->VSSetConstantBuffers(
        4, 1,
        _CB->_constantBuffers[static_cast<UINT>(MeshCBType::BoneMatrix)]
            .GetAddressOf());
    dc->VSSetConstantBuffers(1, 1, _frameCB->_constantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, _frameCB->_constantBuffer.GetAddressOf());

    _staticTransMeshes.resize(2);
    _skelTransMeshes.resize(2);

    _staticOpaqueMesh.resize(2);
    _skelOpaqueMesh.resize(2);

    _staticShadowMesh.resize(2);
    _skelShadowMesh.resize(2);
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
  void SetMainLightDir(DirectionalLight mainLight) { _mainLight = mainLight; }

  void ClassifyPass(MeshBuffer* buff, Matrix world, vector<XMMATRIX> boneMatrix,
                    RenderTypeFlags flags, Color outlineColor)
  {
    if (boneMatrix.empty())
    {
      StaticMesh mesh;
      mesh.buffer = buff;
      mesh.world = world;

      if (buff->flags & RenderPassType::kOpaquePass)
      {
        if (!buff->material->doubleSided)
          _staticOpaqueMesh[0].push_back(mesh);
        else
          _staticOpaqueMesh[1].push_back(mesh);
      }
      if (buff->flags & RenderPassType::kTransparentPass)
      {
        // 뷰 공간의 Z값 계산
        Vector3 worldPos3 = world.Translation(); // 월드 공간에서의 위치
        Vector4 worldPos = {worldPos3.x, worldPos3.y, worldPos3.z,
                            1.f}; // 월드 공간에서의 위치
        Vector4 viewPos =
            Vector4::Transform(worldPos, _camera.view); // 뷰 공간으로 변환
        float viewZ = viewPos.z; // 뷰 공간 Z값 추출
        if (!buff->material->doubleSided)
          _staticTransMeshes[0].insert({{viewZ, max}, mesh});
        else
          _staticTransMeshes[1].insert({{viewZ, max}, mesh});

        max--;
      }

      if (buff->flags & RenderPassType::kShadowPass)
      {
        if (!buff->material->doubleSided)
          _staticShadowMesh[0].push_back(mesh);
        else
          _staticShadowMesh[1].push_back(mesh);
      }
    }
    else
    {
      SkelMesh mesh;
      mesh.buffer = buff;
      mesh.world = world;
      mesh.boneMatrix = boneMatrix;
      if (buff->flags & RenderPassType::kOpaquePass)
      {
        if (!buff->material->doubleSided)
          _skelOpaqueMesh[0].push_back(mesh);
        else
          _skelOpaqueMesh[1].push_back(mesh);
      }
      if (buff->flags & RenderPassType::kTransparentPass)
      {
        // 뷰 공간의 Z값 계산
        Vector3 worldPos3 = world.Translation(); // 월드 공간에서의 위치
        Vector4 worldPos = {worldPos3.x, worldPos3.y, worldPos3.z,
                            1.f}; // 월드 공간에서의 위치
        Vector4 viewPos =
            Vector4::Transform(worldPos, _camera.view); // 뷰 공간으로 변환
        float viewZ = viewPos.z; // 뷰 공간 Z값 추출
        if (!buff->material->doubleSided)
          _skelTransMeshes[0].insert({{viewZ, max}, mesh});
        else
          _skelTransMeshes[1].insert({{viewZ, max}, mesh});

        max--;
      }

      if (buff->flags & RenderPassType::kShadowPass)
      {
        if (!buff->material->doubleSided)
          _skelShadowMesh[0].push_back(mesh);
        else
          _skelShadowMesh[1].push_back(mesh);
      }
    }

    if (flags & RenderType::kOutline)
    {
      if (boneMatrix.empty())
      {
        StaticMesh mesh;
        mesh.buffer = buff;
        mesh.world = world;
        mesh.outlineColor = outlineColor;
        _outLine->_staticMesh.push_back(mesh);
      }
      else
      {
        SkelMesh mesh;
        mesh.buffer = buff;
        mesh.world = world;
        mesh.boneMatrix = boneMatrix;
        mesh.outlineColor = outlineColor;
        _outLine->_skelMesh.push_back(mesh);
      }
    }
  }

  void ProcessPass()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    _pso->ClearBackBufferDSV();
    _pso->SetMainRS();
    _pso->TurnZBufferOn();
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Shadow pass
    this->DrawShadow(dc);
    //--------------------------------------------------------------------------------------//
    // Deferred pass
    this->DrawDeffered(dc);

    //--------------------------------------------------------------------------------------//
    // Transparent pass -> Forward rendering
    this->DrawTransparency(dc);

    //--------------------------------------------------------------------------------------//
    // OutLine
    this->DrawOutline(dc);
    this->DrawBillBoard(dc);
// wireFrame pass -> Forward rendering
#ifdef _DEBUG
    this->DrawGeometryPrimitveWireFrame(dc);
#endif

    // 처리후 클리어
    this->Clear();

    // max값 초기화
    max = std::numeric_limits<int>::max();
  }

  void AddBillBoard(BillboardQuad* quad) { _billboards.push_back(quad); }
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
    macros.clear();
    macros = {{"Shadow", "1"}, {"Skinning", "1"}, {nullptr, nullptr}};
    _vShaders.insert({"SkinningShadow", _compiler->CompileVertexShader(
                                            macros, "shadow_vs_main")});
    macros.clear();
    macros = {{"OutLine", "1"}, {nullptr, nullptr}};
    _vShaders.insert({"StaticOutLine", _compiler->CompileVertexShader(
                                           macros, "outline_vs_main")});
    macros.clear();
    macros = {{"OutLine", "1"}, {"Skinning", "1"}, {nullptr, nullptr}};
    _vShaders.insert({"SkinningOutLine", _compiler->CompileVertexShader(
                                             macros, "outline_vs_main")});
    macros.clear();
    macros = {{"Billboard", "1"}, {nullptr, nullptr}};
    _vShaders.insert({"Billboard", _compiler->CompileVertexShader(
                                       macros, "billboard_vs_main")});

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
    macros.clear();
    macros = {{"OutLine", "1"}, {nullptr, nullptr}};
    _pShaders.insert(
        {"OutLine", _compiler->CompilePixelShader(macros, "outline_ps_main")});
    macros.clear();
    macros = {{"Billboard", "1"}, {nullptr, nullptr}};
    _pShaders.insert({"Billboard", _compiler->CompilePixelShader(
                                       macros, "billboard_ps_main")});
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
  D3D11_VIEWPORT GetMainViewport() { return _pso->GetMainViewPort(); }

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
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
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
      Constant::PixelData data = {.albedoFactor = second};
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
      Constant::PixelData data = {.albedoFactor = second};
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
      Constant::PixelData data = {.albedoFactor = second};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      dc->DrawIndexed(_geometry->cylinder.buffer->nIndices, 0, 0);
    });
  }
#endif

  void DrawShadow(ID3D11DeviceContext* dc)
  {
    _shadow->Prepare();
    dc->PSSetShader(nullptr, nullptr, 0);
    dc->IASetInputLayout(_vShaders["Shadow"]->layout.Get());
    dc->VSSetShader(_vShaders["Shadow"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticShadowMesh[0], [this, dc](StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["SkinningShadow"]->layout.Get());
    dc->VSSetShader(_vShaders["SkinningShadow"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelShadowMesh[0], [this, dc](SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    // static one side meshes
    dc->IASetInputLayout(_vShaders["Shadow"]->layout.Get());
    dc->VSSetShader(_vShaders["Shadow"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticShadowMesh[1], [this, dc](StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["SkinningShadow"]->layout.Get());
    dc->VSSetShader(_vShaders["SkinningShadow"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelShadowMesh[1], [this, dc](SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }

  void DrawDeffered(ID3D11DeviceContext* dc)
  {
    // skybox
    _pso->SetBlendOnEnable(false);
    _pso->SetMainViewPort();
    _pso->SetMainRS();
    _deffered->Prepare(_pso->_backBuffer);
    // dc->IASetInputLayout(_vShaders["SkyBox"]->layout.Get());
    // dc->VSSetShader(_vShaders["SkyBox"]->shader.Get(), nullptr, 0);
    // dc->PSSetShader(_pShaders["SkyBox"]->shader.Get(), nullptr, 0);
    // Constant::World world = {_skyBox->GetMesh()->world.Transpose()};
    //_CB->UpdateContantBuffer(world, MeshCBType::World);
    //_skyBox->Render();
    //  static one side meshes
    dc->PSSetShader(_pShaders["Deffered"]->shader.Get(), nullptr, 0);
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticOpaqueMesh[0], [this, dc](StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelOpaqueMesh[0], [this, dc](SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    // static one side meshes
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticOpaqueMesh[1], [this, dc](StaticMesh mesh) {
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelOpaqueMesh[1], [this, dc](SkelMesh mesh) {
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
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

    dc->PSSetShader(_pShaders["Transparency"]->shader.Get(), nullptr, 0);
    _pso->TurnZBufferOn();
    // static one side meshes
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticTransMeshes[0], [this, dc](const auto& iter) {
      const auto& [z, mesh] = iter;

      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelTransMeshes[0], [this, dc](const auto& iter) {
      const auto& [z, mesh] = iter;
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    _pso->SetCullNone();
    // static one side meshes
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_staticTransMeshes[1], [this, dc](const auto& iter) {
      const auto& [z, mesh] = iter;
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
    // skel one side meshes
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    std::ranges::for_each(_skelTransMeshes[1], [this, dc](const auto& iter) {
      const auto& [z, mesh] = iter;
      Constant::BoneMatrix boneMat = {};
      std::copy(mesh.boneMatrix.begin(), mesh.boneMatrix.end(), boneMat.matrix);

      _CB->UpdateContantBuffer(boneMat, MeshCBType::BoneMatrix);
      dc->VSSetShaderResources(15, 1, mesh.buffer->boneIDSrv.GetAddressOf());
      dc->VSSetShaderResources(16, 1,
                               mesh.buffer->boneWeightsSrv.GetAddressOf());
      dc->IASetVertexBuffers(0, 1, mesh.buffer->vertexBuffer.GetAddressOf(),
                             &(mesh.buffer->stride), &(mesh.buffer->offset));
      dc->IASetIndexBuffer(mesh.buffer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT,
                           0);
      Constant::World world = {mesh.world.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      Constant::PixelData data = {
          .alphaCutoff = mesh.buffer->material->alphaCutoff,
          .metalicFactor = mesh.buffer->material->metallicFactor,
          .roughnessFactor = mesh.buffer->material->roughnessFactor,
          .albedoFactor = mesh.buffer->material->albedoFactor};
      _CB->UpdateContantBuffer(data, MeshCBType::PixelData);
      mesh.buffer->material->PSSetResourceViews(_device);
      dc->DrawIndexed(mesh.buffer->nIndices, 0, 0);
    });
  }

  void DrawOutline(ID3D11DeviceContext* dc)
  {
    // write stencil only nead vertex shader
    _pso->ClearBackBufferDSV();
    _pso->SetStencilWrite();
    dc->PSSetShader(nullptr, nullptr, 0);
    dc->IASetInputLayout(_vShaders["Default"]->layout.Get());
    dc->VSSetShader(_vShaders["Default"]->shader.Get(), nullptr, 0);
    _outLine->WriteStaticStencil(dc);
    dc->IASetInputLayout(_vShaders["Skinning"]->layout.Get());
    dc->VSSetShader(_vShaders["Skinning"]->shader.Get(), nullptr, 0);
    _outLine->WriteSkelStencil(dc);
    _pso->SetStencilRead();
    dc->PSSetShader(_pShaders["OutLine"]->shader.Get(), nullptr, 0);
    dc->IASetInputLayout(_vShaders["StaticOutLine"]->layout.Get());
    dc->VSSetShader(_vShaders["StaticOutLine"]->shader.Get(), nullptr, 0);
    _outLine->DrawStaticOutline(dc);
    dc->IASetInputLayout(_vShaders["SkinningOutLine"]->layout.Get());
    dc->VSSetShader(_vShaders["SkinningOutLine"]->shader.Get(), nullptr, 0);
    _outLine->DrawSkelOutline(dc);
  }

  void DrawBillBoard(ID3D11DeviceContext* dc)
  {
    //_pso->SetMainRS();
    _pso->TurnZBufferOn();
    dc->VSSetShader(_vShaders["Billboard"]->shader.Get(), nullptr, 0);
    dc->IASetInputLayout(_vShaders["Billboard"]->layout.Get());
    dc->PSSetShader(_pShaders["Billboard"]->shader.Get(), nullptr, 0);
    UINT stride = sizeof(Quad::Vertex);
    UINT offset = 0;
    if (!_billboards.empty())
    {
      dc->IASetVertexBuffers(0, 1, _billboards[0]->_vertexBuffer.GetAddressOf(),
                             &stride, &offset);
      dc->IASetIndexBuffer(_billboards[0]->_indexBuffer.Get(),
                           DXGI_FORMAT_R32_UINT, 0);
    }
    std::ranges::for_each(_billboards, [this, dc](BillboardQuad* quad) {
      Matrix s = Matrix::CreateScale(quad->scale);
      
      Vector3 forward = _camera.eye - (Vector3)quad->position;
      forward.Normalize();
      
      Vector3 cameraUp = Vector3::Up;
      Vector3 right = cameraUp.Cross(forward);
      right.Normalize();
      Vector3 up = forward.Cross(right);
      up.Normalize();
      Matrix r =
          Matrix(
            right.x, right.y, right.z, 0.0f,
            up.x, up.y, up.z, 0.0f,
            forward.x, forward.y, forward.z, 0.0f, 
            0.0f, 0.0f, 0.0f, 1.0f);
     

      Matrix t = Matrix::CreateTranslation(quad->position.x, quad->position.y,
                                           quad->position.z);
      Matrix w = s * r * t;
      Constant::World world = {w.Transpose()};
      _CB->UpdateContantBuffer(world, MeshCBType::World);
      quad->material->PSSetResourceViews(_device);
      dc->DrawIndexed(quad->_indexCount, 0, 0);
    });
  }
  void Clear()
  {
    // Transparent meshes
    for (auto& m : _staticTransMeshes)
    {
      m.clear();
    }

    for (auto& m : _skelTransMeshes)
    {
      m.clear();
    }
    // Opaque meshes
    for (auto& meshes : _staticOpaqueMesh)
    {
      meshes.clear();
    }

    for (auto& meshes : _skelOpaqueMesh)
    {
      meshes.clear();
    }

    // Shadow meshes

    for (auto& meshes : _staticShadowMesh)
    {
      meshes.clear();
    }

    for (auto& meshes : _skelShadowMesh)
    {
      meshes.clear();
    }
    _outLine->ClearMeshes();
    _billboards.clear();
#ifdef _DEBUG
    _spheres.clear();
    _boxes.clear();
    _cylinders.clear();
#endif
  }
};
