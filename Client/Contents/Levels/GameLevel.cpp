#include "GameLevel.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/DialogUI/ResultDialogUI.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Resource2DManager/Resource2DManager.h"

void GameLevel::PrepareLevel()
{
  /*mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
  mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
  mapWarehouseMeshHandle =
      LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");*/

  // Cell models load.
  defaultCellModelHandle = LoadModel("Models\\Grid\\Normal\\Grid_Normal.glb");
  placementCellModelHandle =
      LoadModel("Models\\Grid\\CharOver\\Grid_CharOver.glb");
  rangeCellModelHandle = LoadModel("Models\\Grid\\Empty\\Grid_Empty.glb");
  dashCellModelHandle = LoadModel("Models\\Grid\\Dash\\Grid_Dir.glb");
  damageCellModelHandle = LoadModel("Models\\Grid\\Active\\Grid_Active.glb");

  deathIndicatorHandle =
      LoadTexture("Textures\\Indicator_Death.png", TextureType::kAlbedo);
  tabIndicatorHandle =
      LoadTexture("Models\\Indicator\\TabIndicator\\UI_Rotate_01.png",
                  TextureType::kAlbedo);

  // Obstacle model load.
  enemyBrawlerModelHandle =
      LoadModel("Models\\Character\\Enemy\\Enemy_Punch\\Enemy_Punch.glb");
  enemyGunmanModelHandle =
      LoadModel("Models\\Character\\Enemy\\Enemy_Gun\\Enemy_Gun.glb");
  enemyBossModelHandle = LoadModel("Models\\Character\\Enemy\\Enemy_Boss\\Enemy_003.glb");

  allyBrawlerModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Punch\\Player_Punch.glb");
  allySlasherModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Knife\\Player_Knife.glb");
  allyGunmanModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Gun\\Player_Gun.glb");

  characterIndicatorModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Alpha\\Player_Alpha.glb");

  civilianModelHandle =
      LoadModel("Models\\Civilian\\Civilian\\Citizen_001.glb");
  elizaModelHandle = LoadModel("Models\\Civilian\\Eliza.glb");

  allyDirectionIndicatorModelHandle = LoadModel(
      "Models\\Indicator\\AllyDirectionIndicator\\AllyDirectionIndicator.glb");
  enemyDirectionIndicatorModelHandle =
      LoadModel("Models\\Indicator\\EnemyDirectionIndicator\\EnemyDirectionIndi"
                "cator.glb");

  // Set character static data.
  brawlerInactiveIndicatorTextureHandle = LoadTexture(
      "Models\\Indicator\\BrawlerInactiveIndicator\\InactiveIndicator3.png",
      TextureType::kAlbedo);
  brawlerActiveIndicatorTextureHandle = LoadTexture(
      "Models\\Indicator\\BrawlerActiveIndicator\\Indicator3_On.png",
      TextureType::kAlbedo);
  slasherInactiveIndicatorTextureHandle = LoadTexture(
      "Models\\Indicator\\SlasherInactiveIndicator\\InactiveIndicator2.png",
      TextureType::kAlbedo);
  slasherActiveIndicatorTextureHandle = LoadTexture(
      "Models\\Indicator\\SlasherActiveIndicator\\Indicator2_On.png",
      TextureType::kAlbedo);
  gunmanInactiveIndicatorTextureHandle = LoadTexture(
      "Models\\Indicator\\GunmanInactiveIndicator\\InactiveIndicator1.png",
      TextureType::kAlbedo);
  gunmanActiveIndicatorTextureHandle =
      LoadTexture("Models\\Indicator\\GunmanActiveIndicator\\Indicator1_On.png",
                  TextureType::kAlbedo);

  obstacleBox01ModelHandle =
      LoadModel("Models\\Obstacles\\Box01\\OBs_Box01.glb");
  obstacleBox02ModelHandle =
      LoadModel("Models\\Obstacles\\Box02\\OBs_Box02.glb");
  obstacleDrumModelHandle = LoadModel("Models\\Obstacles\\Drum\\OBs_Drum.glb");
  obstacleDrumOldModelHandle =
      LoadModel("Models\\Obstacles\\DrumOld\\OBs_DrumOld.glb");
  obstacleLionModelHandle = LoadModel("Models\\Obstacles\\Lion\\OBs_Lion.glb");
  obstacleSofaModelHandle = LoadModel("Models\\Obstacles\\Sofa\\OBs_Sofa.glb");
  obstacleStoolModelHandle =
      LoadModel("Models\\Obstacles\\Stool\\OBs_Stool.glb");
  obstacleVBoxModelHandle = LoadModel("Models\\Obstacles\\VBox\\OBs_VBox.glb");

  // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\gradient.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Stage_L.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Stage_S.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Order_Deact.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Play_Deact.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Retry_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Retry_Hover.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Confirm_Act.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Confirm_Hover.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Fist.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Fist.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Hover_Fist.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Rush.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Rush.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Hover_Rush.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Act_Gun.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Gun.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Hover_Gun.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\Picture\\PhotoPanel.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Initiative_Gunfire.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Noble.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Despair30.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Despair70.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Joy.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Sad.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Textbox_01.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Textbox_Button.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Selectbox_01.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Selectbox_02.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Selectbox_03.png");

  // Test용 이미지
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\UI_Storage_Deact_Gun.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\X_test.png");
#endif // USED2D
}

void GameLevel::BeginLevel()
{
  //testCamera = world->CreateGameObject<CameraObject>();
  //testCamera->InitCamera(kScreenWidth, kScreenHeight, XM_PIDIV4);
  //testCamera->SetAsMainCamera();
  testCamera = world->CreateGameObject<FixedCamera>();
  
  testCamera->SetCameraPosition({-20.f, 20.f, -20.f, 1.f});
  testCamera->SetFocus({3.f, 0.f, 3.f, 1.f});
  testCamera->SetAsMainCamera();
  // map->Translate(0.6f, -0.01f, 0.8f);
  CreateMap();

  // 여기서 gameManager 만들어줌.
  gameManager = world->CreateGameObject<GameManager>();

#ifdef USED2D
  world->_canvas->CreatePanel<UICursor>(L"Cursor");
  resultDialogUI =
      world->_canvas->CreatePanel<ResultDialogUI>(L"ResultDialogUI");
  inGameUI = world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
  world->_canvas->HidePanel(L"ResultDialogUI");
  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);

#endif // USED2D
}

void GameLevel::CleanupLevel()
{
  transitionUI->FadeOut(2.f);

//  #ifdef USED2D
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\gradient.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Stage_L.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Stage_S.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Order_Act.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Order_Hover.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Order_Deact.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Play_Act.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Play_Hover.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Play_Deact.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Retry_Act.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Retry_Hover.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Confirm_Act.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Confirm_Hover.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Act_Fist.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Deact_Fist.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Hover_Fist.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Act_Rush.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Deact_Rush.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Hover_Rush.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Act_Gun.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Deact_Gun.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Hover_Gun.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\FadeBlack.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "Textures\\Picture\\PhotoPanel.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Initiative_Gunfire.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Noble.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Resistance_Despair30.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Resistance_Despair70.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Resistance_Joy.png");
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\Animation\\Eliza_Resistance_Sad.png");
//
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\FadeBlack.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Textbox_01.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Textbox_Button.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Selectbox_01.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Selectbox_02.png");
//  Resource2DManager::GetInstance()->UnloadSprite("2D\\UI\\UI_Selectbox_03.png");
//
//  // Test용 이미지
//  Resource2DManager::GetInstance()->UnloadSprite(
//      "2D\\UI\\UI_Storage_Deact_Gun.png");
//  Resource2DManager::GetInstance()->UnloadSprite("Textures\\X_test.png");
//#endif // USED2D

	UnloadModel(allyDirectionIndicatorModelHandle);
  UnloadModel(enemyDirectionIndicatorModelHandle);
  UnloadTexture(brawlerInactiveIndicatorTextureHandle);
  UnloadTexture(brawlerActiveIndicatorTextureHandle);
  UnloadTexture(slasherInactiveIndicatorTextureHandle);
  UnloadTexture(slasherActiveIndicatorTextureHandle);
  UnloadTexture(gunmanInactiveIndicatorTextureHandle);
  UnloadTexture(gunmanActiveIndicatorTextureHandle);
  UnloadModel(enemyBrawlerModelHandle);
  UnloadModel(enemyGunmanModelHandle);
  UnloadModel(enemyBossModelHandle);
  UnloadModel(civilianModelHandle);
  UnloadModel(elizaModelHandle);
  UnloadModel(allyBrawlerModelHandle);
  UnloadModel(allySlasherModelHandle);
  UnloadModel(allyGunmanModelHandle);
  UnloadModel(characterIndicatorModelHandle);
  UnloadModel(obstacleBox01ModelHandle);
  UnloadModel(obstacleBox02ModelHandle);
  UnloadModel(obstacleDrumModelHandle);
  UnloadModel(obstacleDrumOldModelHandle);
  UnloadModel(obstacleLionModelHandle);
  UnloadModel(obstacleSofaModelHandle);
  UnloadModel(obstacleStoolModelHandle);
  UnloadModel(obstacleVBoxModelHandle);

  UnloadModel(damageCellModelHandle);
  UnloadModel(dashCellModelHandle);
  UnloadModel(rangeCellModelHandle);
  UnloadModel(placementCellModelHandle);
  UnloadModel(defaultCellModelHandle);

  UnloadTexture(deathIndicatorHandle);
  UnloadTexture(tabIndicatorHandle);
}

void GameLevel::BindDialogLevel(class DialogLevel* dialoglevelptr)
{
  dialoglevel = dialoglevelptr;
}

eBattleResult GameLevel::GetBattleResult()
{
  return _battleResult;
}
void GameLevel::SetBattleResult(eBattleResult br)
{
  _battleResult = br;
  dialoglevel->SetBattleResult(br);
  resultDialogUI->SetPrevBattleResult(br);
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
