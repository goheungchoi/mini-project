#include "GameFramework/UI/UICursor/UICursor.h"
#include "Core/Input/InputSystem.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"
#include "Resource2DManager/Resource2DManager.h"

UICursor::UICursor(World* world) : UIPanel(world)
{
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Cursor_Basic.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Cursor_Snipe.png");

  _sprites[0] = Resource2DManager::GetInstance()
                    ->GetSprite("2D\\UI\\UI_Cursor_Basic.png")
                    .get();
  _sprites[0]->SetScale({0.7, 0.7});

  _sprites[1] = Resource2DManager::GetInstance()
                    ->GetSprite("2D\\UI\\UI_Cursor_Snipe.png")
                    .get();
}

UICursor::~UICursor() {}

void UICursor::Update(float dt)
{
  __super::Update(dt);

  Vector2 curPos = INPUT.GetCurrMouseStateVec();

  if (_selectedIndex == 0)
  {
    _sprites[0]->SetPos({curPos.x - 10, curPos.y});
  }
  else
  {
    _sprites[1]->SetPos({curPos.x - _size.x, curPos.y - _size.y});
  }
}

void UICursor::Render()
{
  _sprites[_selectedIndex]->RenderCursor();
}

void UICursor::SetCursorType(CursorType cursorType)
{
  _selectedIndex = static_cast<int>(cursorType);

  // Optional safety check
  if (_selectedIndex < 0 || _selectedIndex >= 2)
  {
    _selectedIndex = 0;
  }
}

