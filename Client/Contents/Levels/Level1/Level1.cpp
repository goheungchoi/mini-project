#include "Level1.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void Level1::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level1::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
#endif // USED2D
}



void Level1::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMeshHandle);
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 4, 4, kSouth);

  map->CreateCivillianAt(1, 1);

  map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void Level1::TriggerAction() {}
