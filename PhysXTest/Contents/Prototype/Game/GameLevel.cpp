#include "GameLevel.h"

#include "Map.h"

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Museum\\Map_Museum.glb");
}

void GameLevel::BeginLevel()
{
  gameCamera2 = world->CreateGameObject<CameraObject>();
  gameCamera2->InitCamera(1920, 1080);
  gameCamera2->SetAsMainCamera();
  //gameCamera = world->CreateGameObject<FixedCameraObject>();
  //gameCamera->SetCameraPosition({-10.f, 10.f, -10.f});
  //gameCamera->SetFocus({1.f, 1.f, 1.f});
  //gameCamera->SetAsMainCamera();



  // map->Translate(0.6f, -0.01f, 0.8f);
  CreateMap();
  //auto* enemy1 =
  //    map->enemies[0]->GetComponent<RigidbodyComponent>()->GetRigidBody();
  //map->allies[0]->GetComponent<RigidbodyComponent>()->SetCollisionEvent(
  //    enemy1, eCollisionEventType::eCollisionEnter,
  //    [=]() { map->CreateAllyAt(kGunman, 2, 5, kWest); });
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

	 map->CreateEnemyAt(4, 4, kEast);
   //map->CreateEnemyAt(3, 3, kEast);
   map->CreateEnemyAt(2, 1, kEast);


  // map->CreateAllyAt(kBrawler, 4, 5, kWest);
  // map->CreateAllyAt(kSlasher, 3, 5, kWest);
  // map->CreateAllyAt(kGunman, 2, 5, kWest);
	
	// map->CreateCivillianAt(4, 4);

	map->CreateAllyAt(kGunman, 2, 5, kWest);





}

void GameLevel::TriggerAction() {


}
