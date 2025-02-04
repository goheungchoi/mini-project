#include "GameLevel.h"
#include "Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/Prototype/UI/TestUI.h"
#include "Contents/Prototype/UI/MainMenuUI/MainMenuUI.h"
#include "Resource2DManager/Resource2DManager.h"
//#define USED2D

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Map002_Museum.glb");

  // UI Resource Load
#ifdef USED2D 
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleBackGround.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleLogo.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_hover.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_click.png");
#endif // USED2D
}

void GameLevel::BeginLevel()
{
  gameCamera = world->CreateGameObject<FixedCameraObject>();
  gameCamera->SetCameraPosition({-10.f, 10.f, -10.f});
  gameCamera->SetFocus({1.f, 1.f, 1.f});
  gameCamera->SetAsMainCamera();

  CreateMap();

#ifdef USED2D 
  //testObj = world->CreateGameObject<TestGameObject>();
  //world->_canvas->CreatePanel<TestUI>(L"TestUI");
  world->_canvas->CreatePanel<MainMenuUI>(L"MainMenuUI");
#endif // USED2D
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

  /*map->CreateEnemyAt(4, 4, kEast);
  map->CreateEnemyAt(3, 3, kEast);
  map->CreateEnemyAt(2, 1, kEast);

  map->CreateAllyAt(kBrawler, 4, 5, kWest);
  map->CreateAllyAt(kSlasher, 3, 5, kWest);
  map->CreateAllyAt(kGunman, 2, 5, kWest);

  map->CreateCivillianAt(4, 4);*/

  map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void GameLevel::TriggerAction() {}
