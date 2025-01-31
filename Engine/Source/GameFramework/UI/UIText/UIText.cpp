#include "GameFramework/UI/UIText/UIText.h"
#include "Renderer/D2DRenderer/Font/Font.h"

UIText::UIText(World* world) : UIElement(world)
{
}

UIText::~UIText() {}

void UIText::SetText(const wchar_t* format) {}

void UIText::SetFont(const std::wstring& fontName) {}

void UIText::SetColor(Color color) {}
