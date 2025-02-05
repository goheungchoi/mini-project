#include "Level1.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

#define USED2D

void Level1::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");

  // UI Resource Load
  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\MainMissonUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\SubMissonUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\SnipingUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\Btn_Slot.png");
}

void Level1::BeginLevel()
{
  testCamera = world->CreateGameObject<CameraObject>();
  testCamera->InitCamera(kScreenWidth, kScreenHeight, XM_PIDIV4);
  testCamera->SetAsMainCamera();

  // map->Translate(0.6f, -0.01f, 0.8f);
  CreateMap();

#ifdef USED2D
  world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
#endif // USED2D

  // 여기서 gameManager 만들어줌.
  gameManager = world->CreateGameObject<GameManager>();
}

void Level1::CleanupLevel()
{
  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);

  UnloadModel(mapMeshHandle);

  UnloadModel(animTestHandle);
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
