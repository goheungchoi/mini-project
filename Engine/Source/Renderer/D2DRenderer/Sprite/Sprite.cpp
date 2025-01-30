#include "Sprite.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/Resources/Material.h"

Sprite::Sprite(LPCSTR path, Device* pDevice)
{
  _path = path;
  _pDevice = pDevice;

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

void Sprite::Render(DirectX::SpriteBatch* pSpriteBatch)
{
  auto textureSRV = _pTexture->GetResource().Get();

  pSpriteBatch->Draw(textureSRV, _pos);
}




SpriteManager* SpriteManager::m_pInstance = nullptr;


SpriteManager* SpriteManager::GetInstance()
{
  if (m_pInstance == nullptr)
  {
    m_pInstance = new SpriteManager();
  }

  return m_pInstance;
}

std::shared_ptr<Sprite> SpriteManager::GetSprite(LPCSTR path, Device* pDevice)
{
  auto it = _spritePool.find(path);
  if (it != _spritePool.end())
  {
    return it->second;
  }
  else
  {
    auto newSprite = std::make_shared<Sprite>(path, pDevice);
    _spritePool[path] = newSprite;
    return newSprite;
  }
}

void SpriteManager::Destory()
{
  if (m_pInstance)
  {
    // 스프라이트 풀 비우기
    _spritePool.clear();

    // 인스턴스 삭제
    delete m_pInstance;
    m_pInstance = nullptr;
  }
}
