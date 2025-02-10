#include "Map.h"

#include "Core/Input/InputSystem.h"

#include "GameFramework/World/World.h"

#include "Contents/GameObjects/Map/Characters/Brawler/Brawler.h"
#include "Contents/GameObjects/Map/Characters/Civilian/Civilian.h"
#include "Contents/GameObjects/Map/Characters/Gunman/Gunman.h"
#include "Contents/GameObjects/Map/Characters/Slasher/Slasher.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"
#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif
constexpr float kIndicatorScale{0.7f};
extern DirectionalLight _mainLight;
Map::Map(World* world) : GameObject(world)
{
  record.reserve(32);

  // The base models.
  // enemyModelHandle = LoadModel("Models\\Character\\Enemy\\Enemy.glb");
  enemyBrawlerModelHandle =
      LoadModel("Models\\Character\\Enemy\\Enemy_Punch\\Enemy_Punch.glb");
  enemyGunmanModelHandle =
      LoadModel("Models\\Character\\Enemy\\EnemyGunman\\EnemyGunman.glb");

  allyBrawlerModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Punch\\Player_Punch.glb");
  allySlasherModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Knife\\Player_Knife.glb");
  allyGunmanModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Gun\\Player_Gun.glb");

  characterIndicatorModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Alpha\\Player_Alpha.glb");

  civilianModelHandle = LoadModel("Models\\Civilian\\Civilian\\Citizen_001.glb");
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

  obstacleBox01ModelHandle = LoadModel("Models\\Obstacles\\VBox\\OBs_VBox.glb");
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

  Character::enemyModelData = &AccessModelData(enemyGunmanModelHandle);
  Character::enemySkeletonHandle = Character::enemyModelData->skeleton;

  Character::playerModelData = &AccessModelData(allyBrawlerModelHandle);
  Character::playerSkeletonHandle = Character::playerModelData->skeleton;

  // Civilian resources
  Character::civilianModelData = &AccessModelData(civilianModelHandle);
  Character::civilianSkeletonHandle = Character::civilianModelData->skeleton;
  auto civilAnimIt = Character::civilianModelData->animations.begin();

  Character::civilianIdleAnimation = *std::next(civilAnimIt, 0);
  Character::civilianSurrenderAnimation = *std::next(civilAnimIt, 3);
  Character::civilianDeadAnimation = *std::next(civilAnimIt, 1);

  // Animations
  auto animIt = Character::playerModelData->animations.begin();

  Character::deadAnimation = *std::next(animIt, 2);
  Character::idleAnimation = *std::next(animIt, 6);

  Character::brawlerActionAnimation = *std::next(animIt, 8);

  Character::slashReadyAnimation = *std::next(animIt, 1);
  Character::slashAction1Animation = *std::next(animIt, 5);
  Character::slashAction2Animation = *std::next(animIt, 7);

  Character::gunReady1Animation = *std::next(animIt, 3);
  Character::gunReady2Animation = *std::next(animIt, 4);
  Character::gunFireAnimation = *std::next(animIt, 0);

  ModelData& enemyModel = AccessModelData(enemyGunmanModelHandle);

  // Get the player model data.
  ModelData& playerModel = AccessModelData(allyBrawlerModelHandle);

  // Create a grid.
  grid = world->CreateGameObject<GridObject>();
  grid->CreateGrid(6, 6, 1.4f);
  grid->Translate(-0.6f, +0.01f, -0.8f);
  AddChildGameObject(grid);

  // Create walls.
  // Front
  GameObject* wall1 = world->CreateGameObject();
  AddChildGameObject(wall1);
  wall1->SetGameObjectTag("Wall");
  {
    auto* rigidBody = wall1->CreateComponent<RigidbodyComponent>();
    rigidBody->Initialize({5.f, 0, 10.f}, Quaternion::Identity,
                          {10.f, 10.f, 1.f}, ColliderShape::eCubeCollider,
                          false, true, GetWorld()->_phyjixWorld);
    rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
      rigidBody->debugColor = Color(0, 1, 1, 1);
    });
    rigidBody->EnableDebugDraw();
    rigidBody->EnableSimulation();
    rigidBody->GetRigidBody()->DisableSceneQuery();
  }

  // Right
  GameObject* wall2 = world->CreateGameObject();
  AddChildGameObject(wall2);
  wall2->SetGameObjectTag("Wall");
  {
    auto* rigidBody = wall2->CreateComponent<RigidbodyComponent>();
    rigidBody->Initialize({-5.f, 0, 10.f}, Quaternion::Identity,
                          {10.f, 10.f, 1.f}, ColliderShape::eCubeCollider,
                          false, true, GetWorld()->_phyjixWorld);
    rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
      rigidBody->debugColor = Color(0, 1, 1, 1);
    });
    rigidBody->EnableDebugDraw();
    rigidBody->EnableSimulation();
    rigidBody->GetRigidBody()->DisableSceneQuery();
    wall2->RotateAroundYAxis(XM_PIDIV2);
  }

  // Back
  GameObject* wall3 = world->CreateGameObject();
  AddChildGameObject(wall3);
  wall3->SetGameObjectTag("Wall");
  {
    auto* rigidBody = wall3->CreateComponent<RigidbodyComponent>();
    rigidBody->Initialize({-5.f, 0, 2.f}, Quaternion::Identity,
                          {10.f, 10.f, 1.f}, ColliderShape::eCubeCollider,
                          false, true, GetWorld()->_phyjixWorld);
    rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
      rigidBody->debugColor = Color(0, 1, 1, 1);
    });
    rigidBody->EnableDebugDraw();
    rigidBody->EnableSimulation();
    rigidBody->GetRigidBody()->DisableSceneQuery();
    wall3->RotateAroundYAxis(XM_PI);
  }

  // Left
  GameObject* wall4 = world->CreateGameObject();
  AddChildGameObject(wall4);
  wall4->SetGameObjectTag("Wall");
  {
    auto* rigidBody = wall4->CreateComponent<RigidbodyComponent>();
    rigidBody->Initialize({5.f, 0, 2.f}, Quaternion::Identity,
                          {10.f, 10.f, 1.f}, ColliderShape::eCubeCollider,
                          false, true, GetWorld()->_phyjixWorld);
    rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
      rigidBody->debugColor = Color(0, 1, 1, 1);
    });
    rigidBody->EnableDebugDraw();
    rigidBody->EnableSimulation();
    rigidBody->GetRigidBody()->DisableSceneQuery();
    wall4->RotateAroundYAxis(XM_PI + XM_PIDIV2);
  }
}

Map::~Map()
{

  UnloadModel(allyBrawlerModelHandle);
  UnloadModel(enemyGunmanModelHandle);
}

void Map::TurnOnPlacementMode(CharacterType type, Direction dir)
{
  if (isPlacementModeOn)
  {
    return;
  }

  // Turn on the placement mode on grid.
  grid->TurnOnSelectionMode();

  // TODO: Placeholder's model should be transparent.
  switch (type)
  {
  case kBrawler: {
    Brawler* brawler =
        world->CreateGameObjectFromModel<Brawler>(allyBrawlerModelHandle);
    brawler->SetFaction(kAlly);
    brawler->SetDirection(dir);
    brawler->SetPlacementMode(true);
    placeholder = brawler;
  }
  break;
  case kSlasher: {
    Slasher* slasher =
        world->CreateGameObjectFromModel<Slasher>(allySlasherModelHandle);
    slasher->SetFaction(kAlly);
    slasher->SetDirection(dir);
    slasher->SetPlacementMode(true);
    placeholder = slasher;
  }
  break;
  case kGunman: {
    Gunman* gunman =
        world->CreateGameObjectFromModel<Gunman>(allyGunmanModelHandle);
    gunman->SetFaction(kAlly);
    gunman->SetDirection(dir);
    gunman->SetPlacementMode(true);
    placeholder = gunman;
  }
  break;
  }

  TranslatePlaceholder();

  isPlacementModeOn = true;
}

void Map::TurnOffPlacementMode()
{
  if (!isPlacementModeOn)
  {
    return;
  }

  // Turn off the hover mode.
  grid->TurnOffGridHover();

  // Remove the placeholder.
  placeholder->HideOutline();
  placeholder->Destroy();
  placeholder = nullptr;

  // Turn off the placement mode
  isPlacementModeOn = false;
}

void Map::ShowHoveredCharacterRange()
{
  // TODO:
  if (!hoveredCharacter)
    return;

  int w = (int)hoveredCharacter->grid_w;
  int h = (int)hoveredCharacter->grid_h;
  // If the hovered character is not on the grid.
  if (!grid->IsGameObjectAt(hoveredCharacter, w, h))
    return;

  if (!bNeedUpdateAttackRange)
    return;

  switch (hoveredCharacter->type)
  {
  case kBrawler: {
    auto [w_offset, h_offset] = hoveredCharacter->GetGridFrontDirection();
    if (hoveredCharacter->isTargetInRange)
    {
      // Show the damage zone.
      if (CellObject* cell = grid->GetCellObjectAt(w + w_offset, h + h_offset);
          cell)
      {
        cell->SetCellType(CellType_DamageZone);
      }

      // Shows death indicators.
      if (GameObject* gameObject =
              grid->GetGameObjectAt(w + w_offset, h + h_offset);
          gameObject)
      {
        // Mark the death indicator.
        if (gameObject->GetGameObjectTag() == kFactionTags[kAlly] ||
            gameObject->GetGameObjectTag() == kFactionTags[kEnemy] ||
            gameObject->GetGameObjectTag() == kFactionTags[kNeutral])
        {
          Character* character = (Character*)gameObject;
          character->ShowDeathIndicator();
        }
      }
    }
    else
    {
      // Show the range zone.
      if (CellObject* cell = grid->GetCellObjectAt(w + w_offset, h + h_offset);
          cell)
      {
        cell->SetCellType(CellType_RangeZone);
      }
    }
  }
  break;
  case kSlasher: {
    // TODO:
    auto [w_offset, h_offset] = hoveredCharacter->GetGridFrontDirection();
    if (hoveredCharacter->isTargetInRange)
    {
      // Show the damage zone.
      for (int i = 0; i < hoveredCharacter->distanceToTarget; ++i)
      {
        w += w_offset;
        h += h_offset;
        CellObject* cell = grid->GetCellObjectAt(w, h);
        if (!cell)
          break;

        cell->SetCellType(CellType_DashZone);
        cell->SetCellDirection(hoveredCharacter->dir);

        // Shows death indicators.
        if (GameObject* gameObject = grid->GetGameObjectAt(w, h); gameObject)
        {
          // Mark the death indicator.
          if (gameObject->GetGameObjectTag() == kFactionTags[kAlly] ||
              gameObject->GetGameObjectTag() == kFactionTags[kEnemy] ||
              gameObject->GetGameObjectTag() == kFactionTags[kNeutral])
          {
            Character* character = (Character*)gameObject;
            character->ShowDeathIndicator();
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < hoveredCharacter->range; ++i)
      {
        w += w_offset;
        h += h_offset;
        CellObject* cell = grid->GetCellObjectAt(w, h);
        if (!cell)
          break;

        cell->SetCellType(CellType_RangeZone);
      }
    }
  }
  break;
  case kGunman: {
    auto [w_offset, h_offset] = hoveredCharacter->GetGridFrontDirection();
    if (hoveredCharacter->isTargetInRange)
    {
      // Show the damage zone.
      w += w_offset;
      h += h_offset;
      CellObject* cell = grid->GetCellObjectAt(w, h);

      while (cell)
      {
        // Mark the damage zone.
        cell->SetCellType(CellType_DamageZone);

        // Mark the death indicator if any.
        if (GameObject* gameObject = grid->GetGameObjectAt(w, h); gameObject)
        {
          // TODO: Mark the death indicator.
          if (gameObject->GetGameObjectTag() == kFactionTags[kAlly] ||
              gameObject->GetGameObjectTag() == kFactionTags[kEnemy] ||
              gameObject->GetGameObjectTag() == kFactionTags[kNeutral])
          {
            Character* character = (Character*)gameObject;
            character->ShowDeathIndicator();
          }
        }

        // Progress
        w += w_offset;
        h += h_offset;
        cell = grid->GetCellObjectAt(w, h);
      }
    }
    else
    {
      w += w_offset;
      h += h_offset;
      CellObject* cell = grid->GetCellObjectAt(w, h);
      while (cell)
      {
        cell->SetCellType(CellType_RangeZone);

        w += w_offset;
        h += h_offset;
        cell = grid->GetCellObjectAt(w, h);
      }
    }
  }
  break;
  }

  bNeedUpdateAttackRange = false;
  bRangeHided = false;
}

void Map::HideHoveredCharacterRange()
{
  if (bRangeHided)
    return;

  for (uint32_t w = 0; grid->GetCellObjectAt(w, 0) != nullptr; ++w)
  {
    for (uint32_t h = 0; grid->GetCellObjectAt(w, h) != nullptr; ++h)
    {
      if (CellObject* cell = grid->GetCellObjectAt(w, h); cell)
      {
        cell->SetCellType(CellType_Default);
      }

      if (GameObject* gameObject = grid->GetGameObjectAt(w, h); gameObject)
      {
        if (gameObject->GetGameObjectTag() == kFactionTags[kAlly] ||
            gameObject->GetGameObjectTag() == kFactionTags[kEnemy] ||
            gameObject->GetGameObjectTag() == kFactionTags[kNeutral])
        {
          Character* character = (Character*)gameObject;
          character->HideDeathIndicator();
        }
      }
    }
  }

  bRangeHided = true;
}

void Map::TurnOnAssassinationMode()
{
  if (isActionTriggered)
    isAssassinationMode = false;
  else
    isAssassinationMode = true;
}

void Map::TurnOffAssasinationMode()
{
  isAssassinationMode = false;
}

int Map::GetNumEnemies()
{
  return enemies.size();
}

int Map::GetNumAllies()
{
  return allies.size();
}

int Map::GetNumCivilians()
{
  return civilians.size();
}

int Map::GetNumDeadEnemies()
{
  int count = 0;
  for (Character* enemy : enemies)
  {
    count += enemy->isDead;
  }

  return count;
}

int Map::GetNumDeadAllies()
{
  int count = 0;
  for (Character* ally : allies)
  {
    count += ally->isDead;
  }

  return count;
}

int Map::GetNumDeadCivilians()
{
  int count = 0;
  for (Character* civilian : civilians)
  {
    count += civilian->isDead;
  }

  return count;
}

void Map::TriggerAction()
{
  isActionTriggered = true;

  // Record the roster.
  for (Character* enemy : enemies)
  {
    CharacterInfo info{enemy->faction, enemy->type, enemy->dir, enemy->grid_w,
                       enemy->grid_h};
    record.push_back(info);
  }

  for (Character* ally : allies)
  {
    CharacterInfo info{ally->faction, ally->type, ally->dir, ally->grid_w,
                       ally->grid_h};
    record.push_back(info);
  }

  for (Character* civilian : civilians)
  {
    CharacterInfo info{civilian->faction, civilian->type, civilian->dir,
                       civilian->grid_w, civilian->grid_h};
    record.push_back(info);
  }

  // TODO: Execute the assassination target.
  if (assassinationTarget)
    assassinationTarget->Die();

  for (Character* enemy : enemies)
  {
    enemy->TriggerAction();
  }

  for (Character* ally : allies)
  {
    ally->TriggerAction();
  }

  for (Character* civilian : civilians)
  {
    civilian->TriggerAction();
  }
}

void Map::ResetGame()
{
  if (!isActionTriggered)
    return;

  for (Character* enemy : enemies)
  {
    grid->RemoveGameObject(enemy);
    enemy->Destroy();
  }

  for (Character* ally : allies)
  {
    grid->RemoveGameObject(ally);
    ally->Destroy();
  }

  for (Character* civilian : civilians)
  {
    grid->RemoveGameObject(civilian);
    civilian->Destroy();
  }

  grid->ClearGrid();

  enemies.clear();
  allies.clear();
  civilians.clear();

  isActionTriggered = false;
  isPlacementModeOn = false;
  placeholder = nullptr;
  isHoveredCharacterChanged = false;
  hoveredCharacter = nullptr;
  bRangeHided = true;
  isAssassinationMode = false;
  assassinationTarget = nullptr;

  // Restore the record.
  for (auto& info : record)
  {
    switch (info.faction)
    {
    case kAlly:
      CreateAllyAt(info.type, info.w, info.h, info.dir);
      break;
    case kEnemy:
      CreateEnemyAt(info.type, info.w, info.h, info.dir);
      break;
    case kNeutral:
      CreateCivillianAt(info.w, info.h, info.dir);
      break;
    }
  }
  record.clear();
}

void Map::PauseGame()
{
  this->Deactivate();
}

void Map::ResumeGame()
{
  this->Activate();
}

bool Map::IsGameFinished()
{
  bool isAllCharacterFinishedActions{isActionTriggered};

  for (Character* enemy : enemies)
  {
    isAllCharacterFinishedActions &= enemy->IsFinishedAction();
  }

  for (Character* ally : allies)
  {
    isAllCharacterFinishedActions &= ally->IsFinishedAction();
  }

  return isAllCharacterFinishedActions;
}

void Map::CreateEnemyAt(CharacterType type, uint32_t w, uint32_t h,
                        Direction dir)
{
  switch (type)
  {
  case kBrawler: {
    Brawler* brawler =
        world->CreateGameObjectFromModel<Brawler>(enemyBrawlerModelHandle);

    // Bind a direction indicator.
    auto* directionIndicator =
        world->CreateGameObjectFromModel(enemyDirectionIndicatorModelHandle);
    if (directionIndicator)
    {
      brawler->BindDirectionIndicator(directionIndicator);
    }

    // Bind an inactive indicator.
    auto* inactiveIndicator = world->CreateGameObject();
    if (auto* bbComp = inactiveIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerInactiveIndicatorTextureHandle);
    }
    brawler->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerActiveIndicatorTextureHandle);
    }
    brawler->BindActiveIndicator(activeIndicator);

    // Properties
    brawler->SetFaction(kEnemy);
    brawler->SetGridLocation(w, h);
    brawler->SetDirection(dir);
    enemies.push_back(brawler);
  }
  break;
  case kGunman: {
    Gunman* gunman =
        world->CreateGameObjectFromModel<Gunman>(enemyGunmanModelHandle);

    // Bind a direction indicator.
    auto* directionIndicator =
        world->CreateGameObjectFromModel(enemyDirectionIndicatorModelHandle);
    if (directionIndicator)
    {
      gunman->BindDirectionIndicator(directionIndicator);
    }

    // Bind an inactive indicator.
    auto* inactiveIndicator = world->CreateGameObject();
    if (auto* bbComp = inactiveIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanInactiveIndicatorTextureHandle);
    }
    gunman->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanActiveIndicatorTextureHandle);
    }
    gunman->BindActiveIndicator(activeIndicator);

    gunman->SetFaction(kEnemy);
    gunman->SetGridLocation(w, h);
    gunman->SetDirection(dir);
    enemies.push_back(gunman);
  }
  break;
  }
}

void Map::CreateAllyAt(CharacterType type, uint32_t w, uint32_t h,
                       Direction dir)
{
  switch (type)
  {
  case kBrawler: {
    Brawler* brawler =
        world->CreateGameObjectFromModel<Brawler>(allyBrawlerModelHandle);

    // Bind a direction indicator.
    auto* directionIndicator =
        world->CreateGameObjectFromModel(allyDirectionIndicatorModelHandle);
    if (directionIndicator)
    {
      brawler->BindDirectionIndicator(directionIndicator);
    }

    // Bind an inactive indicator.
    auto* inactiveIndicator = world->CreateGameObject();
    if (auto* bbComp = inactiveIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerInactiveIndicatorTextureHandle);
    }
    brawler->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerActiveIndicatorTextureHandle);
    }
    brawler->BindActiveIndicator(activeIndicator);

    // Properties
    brawler->SetFaction(kAlly);
    brawler->SetGridLocation(w, h);
    brawler->SetDirection(dir);
    allies.push_back(brawler);
  }
  break;
  case kSlasher: {
    Slasher* slasher =
        world->CreateGameObjectFromModel<Slasher>(allySlasherModelHandle);

    // Bind a direction indicator.
    auto* directionIndicator =
        world->CreateGameObjectFromModel(allyDirectionIndicatorModelHandle);
    if (directionIndicator)
    {
      slasher->BindDirectionIndicator(directionIndicator);
    }

    // Bind an inactive indicator.
    auto* inactiveIndicator = world->CreateGameObject();
    if (auto* bbComp = inactiveIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(slasherInactiveIndicatorTextureHandle);
    }
    slasher->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(slasherActiveIndicatorTextureHandle);
    }
    slasher->BindActiveIndicator(activeIndicator);

    // Properties
    slasher->SetFaction(kAlly);
    slasher->SetGridLocation(w, h);
    slasher->SetDirection(dir);
    allies.push_back(slasher);
  }
  break;
  case kGunman: {
    Gunman* gunman =
        world->CreateGameObjectFromModel<Gunman>(allyGunmanModelHandle);

    // Bind a direction indicator.
    auto* directionIndicator =
        world->CreateGameObjectFromModel(allyDirectionIndicatorModelHandle);
    if (directionIndicator)
    {
      gunman->BindDirectionIndicator(directionIndicator);
    }

    // Bind an inactive indicator.
    auto* inactiveIndicator = world->CreateGameObject();
    if (auto* bbComp = inactiveIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanInactiveIndicatorTextureHandle);
    }
    gunman->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanActiveIndicatorTextureHandle);
    }
    gunman->BindActiveIndicator(activeIndicator);

    gunman->SetFaction(kAlly);
    gunman->SetGridLocation(w, h);
    gunman->SetDirection(dir);
    allies.push_back(gunman);
  }
  break;
  }
}

void Map::CreateCivillianAt(uint32_t w, uint32_t h, Direction dir, bool isEliza)
{
  // TODO: Civilian model
  Civilian* civilian;

  if (isEliza)
  {
    civilian = world->CreateGameObjectFromModel<Civilian>(civilianModelHandle);
  }
  else
  {
    civilian = world->CreateGameObjectFromModel<Civilian>(civilianModelHandle);
  }

  civilian->SetGridLocation(w, h);
  civilian->SetDirection(dir);
  civilians.push_back(civilian);
}

void Map::CreateObstacleAt(ObstacleType type, uint32_t w, uint32_t h,
                           Direction dir)
{
  Obstacle* obstacle{nullptr};
  switch (type)
  {
  case ObstacleType_Stool:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleStoolModelHandle);
    break;
  case ObstacleType_Box01:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleBox01ModelHandle);
    break;
  case ObstacleType_Box02:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleBox02ModelHandle);
    break;
  case ObstacleType_Drum:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleDrumModelHandle);
    break;
  case ObstacleType_DrumOld:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleDrumOldModelHandle);
    break;
  case ObstacleType_VBox:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleVBoxModelHandle);
    break;
  case ObstacleType_Lion:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleLionModelHandle);
    break;
  case ObstacleType_Sofa:
    obstacle =
        world->CreateGameObjectFromModel<Obstacle>(obstacleSofaModelHandle);
    break;
  }

  obstacle->SetObstacleType(type);
  obstacle->SetGridLocation(w, h);
  obstacle->SetDirection(dir);
}

void Map::PlaceCharacterIndicatorAt(uint32_t w, uint32_t h,
                                    Direction dir)
{
  // TODO: 
  Slasher* slasher =
      world->CreateGameObjectFromModel<Slasher>(characterIndicatorModelHandle);

  slasher->animator->PauseAnimation();

  indicatorPosition = {w, h};

  auto [pos_x, pos_z] = grid->GetActualPositionAt(w, h);

  // Apply global transformation
  XMVECTOR pos{pos_x - 0.6f, 0.f, pos_z - 0.8f, 1.f};

  // Bind an inactive indicator.
  tempInactiveIndicator = world->CreateGameObject();
  if (auto* bbComp =
          tempInactiveIndicator->CreateComponent<BillboardComponent>();
      bbComp)
  {
    bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
    bbComp->SetTexture(slasherInactiveIndicatorTextureHandle);
  }
  slasher->BindInactiveIndicator(tempInactiveIndicator);
  tempInactiveIndicator->SetVisible();

  slasher->SetTranslation(pos);
  slasher->SetDirection(dir);
  slasher->DisableHover();
  slasher->SetPlacementMode(true);

  characterIndicator = slasher;

  AddChildGameObject(slasher);
}

void Map::DeleteCharacterFromMap(Character* character)
{
  grid->RemoveGameObject(character);
  switch (character->faction)
  {
  case Faction::kAlly: {
    auto it = std::remove(allies.begin(), allies.end(), character);
    allies.erase(it, allies.end());
  }
  break;
  case Faction::kEnemy: {
    auto it = std::remove(enemies.begin(), enemies.end(), character);
    enemies.erase(it, enemies.end());
  }
  break;
  case Faction::kNeutral: {
    auto it = std::remove(civilians.begin(), civilians.end(), character);
    civilians.erase(it, civilians.end());
  }
  break;
  }
}

void Map::OnAwake()
{
  // Translate({-4.f, 0.f, -4.f});
}

void Map::Update(float dt)
{
  // Detect hovered character changes.
  if (hoveredCharacter)
  {
    if (prevHoveredCharacter != hoveredCharacter)
      isHoveredCharacterChanged = true;
    else
      isHoveredCharacterChanged = false;
  }
  else
  {
    isHoveredCharacterChanged = false;
  }

  // Transparent indicator.
  if (characterIndicator)
  {
    DetectPlacementAtIndicator();
  }
  
  // Rotate this map.
  if (INPUT.IsKeyPress(Key::Q) || INPUT.IsKeyDown(Key::Q))
  {
    mapRot -= dt;
  }
  if (INPUT.IsKeyPress(Key::E) || INPUT.IsKeyDown(Key::E))
  {
    mapRot += dt;
  }
  float oneDegree = XM_PIDIV2 / 90.f;
  float maxAngle = oneDegree * 27.2;
  mapRot = std::clamp(mapRot, -maxAngle, maxAngle);
  parent->SetRotationAroundYAxis(mapRot);

  // Rotate light direction
  XMMATRIX rotationMat = XMMatrixRotationY(mapRot);
  XMVECTOR rotatedPoint = XMVector3Transform(lightStartPoint, rotationMat);
  float newX = XMVectorGetX(rotatedPoint);
  float newY = XMVectorGetY(rotatedPoint);
  float newZ = XMVectorGetZ(rotatedPoint);
  XMVECTOR newLightPoint = {newX, newY, newZ};
  _mainLight.direction = newLightPoint;
  // Action mode
  if (isActionTriggered)
  {
    grid->TurnOffSelectionMode();
    grid->TurnOffGridHover();

    // TODO: Remove a simulating character.

    if (INPUT.IsKeyPress(Key::R))
    {
      ResetGame();
    }
  }
  // Placement mode
  else if (isPlacementModeOn)
  {
    grid->TurnOnSelectionMode();
    grid->TurnOnGridHover();

    // If a placeholder is not set
    if (!placeholder)
    {
      isPlacementModeOn = false;
    }

    TranslatePlaceholder();

    placeholder->ShowOutline();

    // Cancel placement mode.
    if (INPUT.IsKeyPress(Key::Escape) || INPUT.IsKeyPress(MouseState::RB))
    {
      if (tmp)
      {
        CreateAllyAt(tmp->type, tmp->w, tmp->h, tmp->dir);
        tmp.reset();
      }

      if (placeholder != nullptr)
      {
        deleteCharType = placeholder->type;
      }

      TurnOffPlacementMode();
    }
    // Change the direction of the placeholder.
    else if (INPUT.IsKeyPress(Key::Tab))
    {
      SoundManager::PlaySound(SoundList::Placeholder_Rotation);
      uint32_t dir = placeholder->GetDirection();
      placeholder->SetDirection((Direction)((dir + 1) % kNumDirections));
    }
    // Place the character.
    else if (INPUT.IsKeyPress(MouseState::LB))
    {
      if (grid->selectedCell)
      {
        SoundManager::PlaySound(SoundList::Grid_Placement_Click);

        // Detect which grid cell is pointed.
        auto [w, h] = grid->selectedCell->GetCellPosition();

        // Create a character at the cell
        CreateAllyAt(placeholder->type, w, h, placeholder->dir);

        // Turn off the placement mode
        TurnOffPlacementMode();
      }
    }
  }
  // Selection mode
  else
  {
    grid->TurnOnSelectionMode();
    grid->TurnOffGridHover();

    if (prevHoveredCharacter)
    {
      prevHoveredCharacter->HideOutline();
    }

    if (hoveredCharacter)
    {
      hoveredCharacter->ShowOutline();
    }

    if (hoveredCharacter)
    {
      // Assasination mode.
      if (isAssassinationMode)
      {
        if (INPUT.IsKeyPress(MouseState::LB))
        {
          if (hoveredCharacter->faction == Faction::kEnemy)
          {
            SoundManager::PlaySound(SoundList::Snipping_Selection);
            // Register the target to be assassinated,
            // only if in assassination mode.
            assassinationTarget = hoveredCharacter;
            isAssassinationMode = false;
          }
        }
        else if (INPUT.IsKeyPress(MouseState::RB))
        {
          isAssassinationMode = false;
        }
      }
      // Default Selection mode.
      else
      {
        // TODO: Show the distance and range
        if (isHoveredCharacterChanged)
        {
          HideHoveredCharacterRange();
        }

        ShowHoveredCharacterRange();

        // Right Click
        if (INPUT.IsKeyPress(MouseState::RB))
        {
          // Remove the character
          if (hoveredCharacter->faction == Faction::kAlly)
          {
            DeleteCharacterFromMap(hoveredCharacter);
            deleteCharType = hoveredCharacter->type;
            hoveredCharacter->Destroy();
          }
          // Cancel the assassination.
          else if (hoveredCharacter->faction == Faction::kEnemy)
          {
            if (assassinationTarget == hoveredCharacter)
            {
              SoundManager::PlaySound(SoundList::Snipping_Selection);
              // Register the target to be assassinated,
              // only if in assassination mode.
              assassinationTarget = nullptr;
            }
          }
        }
        // Left click
        else if (INPUT.IsKeyPress(MouseState::LB))
        {
          // Character selection -> Placement mode.
          // Find if any "ally character" is selected.
          if (hoveredCharacter->faction == Faction::kAlly)
          {
            // Check the type of the character.
            CharacterType type = hoveredCharacter->type;

            // Check the direction of the character.
            Direction dir = hoveredCharacter->dir;

            // Generate temp info
            tmp = CharacterInfo{.faction = kAlly,
                                .type = type,
                                .dir = dir,
                                .w = hoveredCharacter->grid_w,
                                .h = hoveredCharacter->grid_h};

            // Remove the selected character from the grid.
            DeleteCharacterFromMap(hoveredCharacter);

            // Destroy the selected character.
            hoveredCharacter->Destroy();

            // Nullify the pointer.
            hoveredCharacter = nullptr;

            // Turn on the placement mode.
            TurnOnPlacementMode(type, dir);
          }
        }
      }
    }
    else
    {
      // TODO: Turn off the mode.
      if (!bRangeHided)
      {
        HideHoveredCharacterRange();
      }
    }

    if (INPUT.IsKeyPress(Key::Space))
    {
      TriggerAction();
      return;
    }

    if (INPUT.IsKeyPress(Key::D1))
    {
      TurnOnPlacementMode(kBrawler, kNorth);
      return;
    }

    if (INPUT.IsKeyPress(Key::D2))
    {
      TurnOnPlacementMode(kSlasher, kNorth);
      return;
    }

    if (INPUT.IsKeyPress(Key::D3))
    {
      TurnOnPlacementMode(kGunman, kNorth);
      return;
    }
  }

  // Show death indicator.
  if (assassinationTarget)
  {
    assassinationTarget->ShowDeathIndicator();
  }

  // Reset the hovered character.
  prevHoveredCharacter = hoveredCharacter;
  hoveredCharacter = nullptr;
}

void Map::PostUpdate(float dt)
{
  // hoveredCharacter = nullptr;
}

void Map::OnRender()
{
#ifdef _DEBUG
  if (ImGui::Begin("mapPos"))
  {
    ImGui::SliderFloat3("mapPos", mapPos, -10.f, 10.f);
  }
  ImGui::End();
  XMVECTOR mp = {mapPos[0], mapPos[1], mapPos[2]};
  SetTranslation(mp);
#ifdef _DEBUG
  if (ImGui::Begin("main Light"))
  {
    float _mainLightDir[3] = {_mainLight.direction.x, _mainLight.direction.y,
                              _mainLight.direction.z};
    ImGui::SliderFloat3("direction", _mainLightDir, -1.f, 1.f);
    _mainLight.direction.x = _mainLightDir[0];
    _mainLight.direction.y = _mainLightDir[1];
    _mainLight.direction.z = _mainLightDir[2];
    float _mainLightColor[4] = {_mainLight.radiance.x, _mainLight.radiance.y,
                                _mainLight.radiance.z, 1.f};
    ImGui::ColorEdit3("Color", _mainLightColor);
    float _mainLightIntencity = _mainLight.radiance.w;
    ImGui::SliderFloat("intencity", &_mainLightIntencity, 0.f, 1.f);
    _mainLight.radiance.x = _mainLightColor[0];
    _mainLight.radiance.y = _mainLightColor[1];
    _mainLight.radiance.z = _mainLightColor[2];
    _mainLight.radiance.w = _mainLightIntencity;
  }
  ImGui::End();
#endif
#endif // !_DEBUG
}

XMVECTOR Map::GetCursorPosition() const
{
  // Follow the mouse pointer position.
  // y = 0, intersection between mouse pointer ray and p = (x, 0, z), n =
  // (0, 1, 0) plane. The placeholder character is translated to the
  // intersection point.
  Vector2 mousePos{(float)INPUT.GetCurrMouseState().x,
                   (float)INPUT.GetCurrMouseState().y};
  Ray cursorRay = world->GetScreenCursorRay(mousePos);
  Plane xzPlane{{0, 0, 0}, {0, 1, 0}};

  float t;
  cursorRay.Intersects(xzPlane, t);

  XMVECTOR intersection = cursorRay.position + cursorRay.direction * t;
  return intersection;
}

void Map::TranslatePlaceholder()
{
  if (!placeholder)
    return;

  // Detect if grid cell is pointed.
  if (grid->selectedCell)
  {
    auto [w, h] = grid->selectedCell->GetCellPosition();
    auto [x, z] = grid->GetActualPositionAt(w, h);

    XMVECTOR translate =
        XMVector3Transform({x, 0, z}, grid->GetWorldTransform());

    placeholder->SetTranslation(translate);
  }
  else
  {
    // If the cursor is not on the grid,
    // follow the cursor pos.
    XMVECTOR cursorPos = GetCursorPosition();
    placeholder->SetTranslation(cursorPos);
  }
}

void Map::DetectPlacementAtIndicator()
{
  if (characterIndicator && tempInactiveIndicator)
  {
    if (isActionTriggered)
    {
      HideCharacterIndicator();
      return;
    }

    if (grid->GetGameObjectAt(indicatorPosition.first,
                              indicatorPosition.second))
    {
      HideCharacterIndicator();
    }
    else
    {
      ShowCharacterIndicator();
    }
  }
}

void Map::ShowCharacterIndicator()
{
  tempInactiveIndicator->SetVisible();
  for (auto* child : characterIndicator->childrens)
  {
    if (auto* skelMesh = child->GetComponent<SkeletalMeshComponent>(); skelMesh)
    {
      skelMesh->SetVisible(true);
    }
  }
}

void Map::HideCharacterIndicator()
{
  tempInactiveIndicator->SetInvisible();
  for (auto* child : characterIndicator->childrens)
  {
    if (auto* skelMesh = child->GetComponent<SkeletalMeshComponent>(); skelMesh)
    {
      skelMesh->SetVisible(false);
    }
  }
}

void Map::AssassinateTarget()
{
  if (assassinationTarget)
  {
    assassinationTarget->Die();
    assassinationTarget = nullptr;
  }
}
