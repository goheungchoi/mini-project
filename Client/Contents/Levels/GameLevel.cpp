#include "GameLevel.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void GameLevel::BeginLevel()
{
  testCamera = world->CreateGameObject<CameraObject>();
  testCamera->InitCamera(kScreenWidth, kScreenHeight, XM_PIDIV4);
  testCamera->SetAsMainCamera();

  // map->Translate(0.6f, -0.01f, 0.8f);
  CreateMap();

  // 여기서 gameManager 만들어줌.
  gameManager = world->CreateGameObject<GameManager>();
}

void GameLevel::CleanupLevel()
{
  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);

  UnloadModel(mapMeshHandle);

  UnloadModel(animTestHandle);
}

void GameLevel::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMeshHandle);
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 4, 4, kSouth);
  map->CreateEnemyAt(kGunman, 3, 2, kEast);
  map->CreateEnemyAt(kGunman, 2, 4, kEast);

  // map->CreateAllyAt(kBrawler, 4, 5, kWest);
  // map->CreateAllyAt(kSlasher, 3, 5, kWest);
  // map->CreateAllyAt(kGunman, 2, 5, kWest);

  map->CreateCivillianAt(1, 1);

  map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void GameLevel::TriggerAction() {}
