#include "GameLevel.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "Resource2DManager/Resource2DManager.h"

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");

  // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Stage_L.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Stage_S.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Deact.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Deact.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Fist.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Fist.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Rush.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Rush.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Gun.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Gun.png");
#endif // USED2D
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

#ifdef USED2D
  world->_canvas->CreatePanel<UICursor>(L"Cursor");
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

  map->CreateEnemyAt(kGunman, 4, 4, kSouth);
  map->CreateEnemyAt(kGunman, 3, 2, kEast);
  map->CreateEnemyAt(kGunman, 2, 4, kEast);

  map->CreateCivillianAt(4, 1);

  map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void GameLevel::TriggerAction() {}
