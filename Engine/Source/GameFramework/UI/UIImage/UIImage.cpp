#include "GameFramework/UI/UIImage/UIImage.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"

UIImage::UIImage(World* world) : UIElement(world) {}

UIImage::~UIImage() {}

void UIImage::SetSprite(LPCSTR path, Vector2 pos)
{
  _sprite = _SpriteManager.GetSprite(path).get();
  _size = _sprite->GetTextureSize();
  _sprite->SetPos(pos);
}