#include "GameLevel.h"

#include "Map.h"

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
}

void GameLevel::BeginLevel()
{
  gameCamera = world->CreateGameObject<FixedCameraObject>();
  gameCamera->SetCameraPosition({-10.f, 10.f, -10.f});
  gameCamera->SetFocus({1.f, 1.f, 1.f});
  gameCamera->SetAsMainCamera();

  
  // map->Translate(0.6f, -0.01f, 0.8f);
  CreateMap();
}

void GameLevel::CleanupLevel()
{
  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);

  UnloadModel(mapMeshHandle);

  UnloadModel(animTestHandle);
}

void GameLevel::CreateMap() {
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMeshHandle);
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

	// map->CreateEnemyAt(4, 4, kEast);
  // map->CreateEnemyAt(3, 3, kEast);
  // map->CreateEnemyAt(2, 1, kEast);

  // map->CreateAllyAt(kBrawler, 4, 5, kWest);
  // map->CreateAllyAt(kSlasher, 3, 5, kWest);
  // map->CreateAllyAt(kGunman, 2, 5, kWest);
	
	// map->CreateCivillianAt(4, 4);

	map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void GameLevel::TriggerAction() {


}
