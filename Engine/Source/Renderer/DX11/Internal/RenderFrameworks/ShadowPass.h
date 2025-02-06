#pragma once
#include "../Common.h"
#include "../Descriptions.h"
#include "../Device.h"
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
class ShadowPass
{
private:
  // 8192 8192
  ComPtr<ID3D11Texture2D> _shadowMap = nullptr;
  ComPtr<ID3D11ShaderResourceView> _shadowSRV = nullptr;
  ComPtr<ID3D11DepthStencilView> _shadowDSV = nullptr;
  D3D11_VIEWPORT _shadowViewPort = {};
  Device* _device = nullptr;
  // VARIABLE
  float _forwardDist = 0.f;
  float _upLookAtDist = 310.764f;

  float _nearPlane = 150.f;
  float _farPlane = 10000.f;
public:
  Matrix View;
  Matrix Projection;

public:
  /**
   * @brief Make shadow depth Texture and srv, dsv
   * @param device
   * @param width ShadowMap Width
   * @param height ShadowMap height
   */
  ShadowPass(Device* device, int width, int height) : _device{device}
  {
    UINT flags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    D3D11_TEXTURE2D_DESC texDesc = CreateTexture2DDesc(
        width, height, DXGI_FORMAT_R32_TYPELESS, 1, flags, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, NULL,
                                               _shadowMap.GetAddressOf()));
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    HR_T(_device->GetDevice()->CreateDepthStencilView(
        _shadowMap.Get(), &dsvDesc, _shadowDSV.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _shadowMap.Get(), &srvDesc, _shadowSRV.GetAddressOf()));
    _shadowViewPort = {.TopLeftX = 0,
                       .TopLeftY = 0,
                       .Width = static_cast<float>(width),
                       .Height = static_cast<float>(height),
                       .MinDepth = 0.f,
                       .MaxDepth = 1.f};
  }
  ~ShadowPass() {}

public:
#ifdef _DEBUG
  void UpdateVarialbe()
  {
    if (ImGui::Begin("Shadow"))
    {
      ImGui::SliderFloat("forwadDist", &_forwardDist, 0.f, 100.f);
      ImGui::SliderFloat("UpLookAtDist", &_upLookAtDist, 0.f, 500.f);
      ImGui::SliderFloat("NearPlane", &_nearPlane, 1.f, 1000.f);
      ImGui::SliderFloat("FarPlane", &_farPlane, 1000.f, 10000.f);
      ImTextureID imgID = (ImTextureID)(uintptr_t)_shadowSRV.Get();
      ImGui::Image(imgID, ImVec2(400, 400));
    }
    ImGui::End();
  }
#endif
  void CalculateMatrix(const Matrix& view, const Vector4& eye,
                       const Vector4& lightDir)
  {
    Vector3 _lightDir = {lightDir.x, lightDir.y, lightDir.z};
    _lightDir.Normalize();
    Vector3 viewForward = view.Forward();
    Vector4 shadowLookAt = eye + viewForward * _forwardDist;
    Vector4 shadowPos = shadowLookAt + (-_lightDir * _upLookAtDist);
    Vector3 up = {0.f, 1.f, 0.f};
    Vector3 forward;
    forward.x = shadowLookAt.x - shadowPos.x;
    forward.y = shadowLookAt.y - shadowPos.y;
    forward.z = shadowLookAt.z - shadowPos.z;
    forward.Normalize();
    if (fabs(forward.y) > 0.99)
    {
      up = {1, 0, 0};
    }
    Vector3 right = up.Cross(forward);
    right.Normalize();
    up = forward.Cross(right);
    up.Normalize();
    float x = -right.Dot(Vector3(shadowPos));
    float y = -up.Dot(Vector3(shadowPos));
    float z = -forward.Dot(Vector3(shadowPos));
    View = Matrix(
      right.x, up.x, forward.x, 0.0f,
      right.y, up.y, forward.y, 0.0f, 
      right.z, up.z, forward.z, 0.0f,
      x, y, z, 1.0f);

    Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1,
                                                   _nearPlane, _farPlane);
  }
  void Prepare()
  {
    ID3D11RenderTargetView* nullrtv = nullptr;
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->ClearDepthStencilView(_shadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
    dc->OMSetRenderTargets(0, &nullrtv, _shadowDSV.Get());
    dc->RSSetViewports(1, &_shadowViewPort);
  }
  void SetDepthSRV()
  {
    _device->GetImmContext()->PSSetShaderResources(9, 1,
                                                   _shadowSRV.GetAddressOf());
  }
  void ClearSRV()
  { 
    ID3D11ShaderResourceView* srv = nullptr;
    _device->GetImmContext()->PSSetShaderResources(9, 1, &srv);
  }
};