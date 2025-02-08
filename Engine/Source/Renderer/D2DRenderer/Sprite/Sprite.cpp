#include "Sprite.h"
#include "Renderer/D2DRenderer/D2DRenderer.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/Resources/Material.h"

Device* Sprite::_pDevice = nullptr;
D2DRenderer* Sprite::_pD2DRenderer = nullptr;

Sprite::Sprite(LPCSTR path)
{
  _path = path;

  Init(path);
}

Sprite::~Sprite()
{
  UnInit();
}

void Sprite::Init(LPCSTR path)
{
  // Texture 생성
  TextureHandle handle = LoadTexture(path, TextureType::kUnknown);
  TextureData data;
  data = AccessTextureData(handle);
  _pTexture = Texture::CreateSRV(_pDevice, data);

  _textureSize = CalculateTextureSize();
}

void Sprite::UnInit()
{
  // Texture의 해제
  if (_pTexture)
  {
    SAFE_RELEASE(_pTexture);
  }
}

Vector2 Sprite::CalculateTextureSize()
{
  if (_pTexture == nullptr)
    return Vector2(0, 0);

  ID3D11Resource* resource = nullptr;
  _pTexture->GetResource()->GetResource(&resource);

  ID3D11Texture2D* tex2D = nullptr;
  HRESULT hr = resource->QueryInterface<ID3D11Texture2D>(&tex2D);
  if (FAILED(hr))
  {
    resource->Release();
    return Vector2(0, 0);
  }

  D3D11_TEXTURE2D_DESC desc;
  tex2D->GetDesc(&desc);

  tex2D->Release();
  resource->Release();

  return Vector2(desc.Width, desc.Height);
}

void Sprite::Render()
{

  _pD2DRenderer->_d2dRenderQueue.AddRender2DCmd(
      [=]() {
        auto textureSRV = _pTexture->GetResource().Get();

        _pD2DRenderer->_pSpriteBatch->Draw(
            textureSRV, _pos, nullptr, _maskColor * _opacity,
                          0.0f, DirectX::SimpleMath::Vector2::Zero, _scale);
      },
      PassType2D::SPRITE_BATCH // SpriteBatch 전용 패스 사용
  );
}

void Sprite::Render(Vector4 rect)
{
  _pD2DRenderer->_d2dRenderQueue.AddRender2DCmd(
      [=]() {
        auto textureSRV = _pTexture->GetResource().Get();
        const RECT* _srcRect = new RECT(rect.x, rect.y, rect.z, rect.w);
        _pD2DRenderer->_pSpriteBatch->Draw(
            textureSRV, _pos, _srcRect, _maskColor * _opacity, 0.0f,
            DirectX::SimpleMath::Vector2::Zero, _scale);
      },
      PassType2D::SPRITE_BATCH // SpriteBatch 전용 패스 사용
  );
}

void Sprite::RenderCursor()
{
  _pD2DRenderer->_d2dRenderQueue.SetCursorRenderCmd(
      [=]() {
        auto textureSRV = _pTexture->GetResource().Get();

        _pD2DRenderer->_pSpriteBatch->Draw(
            textureSRV, _pos, nullptr, DirectX::Colors::White * _opacity, 0.0f,
            DirectX::SimpleMath::Vector2::Zero, _scale);
      });
}

void Sprite::RenderTransition()
{
  _pD2DRenderer->_d2dRenderQueue.SetTransitionRenderCmd([=]() {
    auto textureSRV = _pTexture->GetResource().Get();

    _pD2DRenderer->_pSpriteBatch->Draw(
        textureSRV, _pos, nullptr, DirectX::Colors::White * _opacity, 0.0f,
        DirectX::SimpleMath::Vector2::Zero, _scale);
  });
}