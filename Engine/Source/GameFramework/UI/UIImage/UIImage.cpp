#include "GameFramework/UI/UIImage/UIImage.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"

UIImage::UIImage(World* world) : UIElement(world) {}

UIImage::~UIImage() {}

void UIImage::SetSprite(LPCSTR path)
{
  _sprite = new Sprite(path);
}

void UIImage::Render() {}
