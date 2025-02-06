#include "Level2.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void Level2::PrepareLevel()
{
}

void Level2::BeginLevel()
{
  world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
}

void Level2::CleanupLevel() {}

void Level2::CreateMap() {}

void Level2::TriggerAction() {}
