#include "GameFramework/UI/UIImage/UIImage.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"
#include "Resource2DManager/Resource2DManager.h"

UIImage::UIImage(World* world) : UIElement(world) {}

UIImage::~UIImage() {}

void UIImage::Render()
{
  if (_status == EStatus_Active)
  {
    _sprite->Render();
  }
}

void UIImage::SetSprite(LPCSTR path, Vector2 pos)
{
  _sprite = Resource2DManager::GetInstance()->GetSprite(path).get();
  _size = _sprite->GetTextureSize();
  _sprite->SetPos(pos);
}

void UIImage::SetPosition(Vector2 pos)
{
  _sprite->SetPos(pos);
}
