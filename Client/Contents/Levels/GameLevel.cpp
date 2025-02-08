#include "GameLevel.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "Resource2DManager/Resource2DManager.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"

void GameLevel::PrepareLevel()
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  handGunHandle = LoadModel("Models\\HandGun\\HandGun.glb");

  mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
  mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
  mapWarehouseMeshHandle =
      LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");

  OBsStoolHandle = LoadModel("Models\\Obstacles\\OBs_Stool.glb");
  OBsBox02Handle = LoadModel("Models\\Obstacles\\OBs_Box02.glb");

  // Obstacle model load.
  LoadModel("Models\\Obstacles\\VBox\\OBs_VBox.glb");
  LoadModel("Models\\Obstacles\\Box02\\OBs_Box02.glb");
  LoadModel("Models\\Obstacles\\Drum\\OBs_Drum.glb");
  LoadModel("Models\\Obstacles\\DrumOld\\OBs_DrumOld.glb");
  LoadModel("Models\\Obstacles\\Lion\\OBs_Lion.glb");
  LoadModel("Models\\Obstacles\\Sofa\\OBs_Sofa.glb");
  LoadModel("Models\\Obstacles\\Stool\\OBs_Stool.glb");
  LoadModel("Models\\Obstacles\\VBox\\OBs_VBox.glb");

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

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Retry_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Confirm_Act.png");

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
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");


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
  inGameUI = world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);

#endif // USED2D
}

void GameLevel::CleanupLevel()
{
  transitionUI->FadeOut(2.f);


  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);

  UnloadModel(mapBarMeshHandle);
  UnloadModel(mapMuseumMeshHandle);
  UnloadModel(mapWarehouseMeshHandle);

  UnloadModel(animTestHandle);
}

void GameLevel::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 4, 4, kSouth);
  map->CreateEnemyAt(kGunman, 3, 2, kEast);
  map->CreateEnemyAt(kGunman, 2, 4, kEast);

  map->CreateCivillianAt(4, 1);

  map->CreateAllyAt(kGunman, 2, 5, kWest);
}

void GameLevel::TriggerAction() {}
