#include "Level2.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/DialogUI/DialogUI.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "GameFramework/UI/UICursor/UICursor.h"

void Level2::PrepareLevel()
{
  // UI Resource Load
}

void Level2::BeginLevel()
{
#ifdef USED2D
  world->_canvas->CreatePanel<DialogUI>(L"ElizaDialog");
world->_canvas->CreatePanel<UICursor>(L"Cursor");
  world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");


#endif // USED2D
}

void Level2::CleanupLevel() {}

void Level2::CreateMap() {}

void Level2::TriggerAction() {}
