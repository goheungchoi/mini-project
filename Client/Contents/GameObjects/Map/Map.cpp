#include "Map.h"

#include "Core/Input/InputSystem.h"

#include "GameFramework/World/World.h"

#include "Contents/GameObjects/Map/Characters/Brawler/Brawler.h"
#include "Contents/GameObjects/Map/Characters/Civilian/Civilian.h"
#include "Contents/GameObjects/Map/Characters/Gunman/Gunman.h"
#include "Contents/GameObjects/Map/Characters/Slasher/Slasher.h"

constexpr float kIndicatorScale{0.7f};

Map::Map(World* world) : GameObject(world)
{
  record.reserve(32);

  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");

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

  civilianModelHandle = LoadModel("Models\\Civilian\\Eliza.glb");

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
  Character::slashActionAnimation = *std::next(animIt, 5);

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

  // Turn of the selection mode on grid
  grid->TurnOffSelectionMode();

  // Remove the placeholder.
  placeholder->Destroy();
  placeholder = nullptr;

  // Turn off the placement mode
  isPlacementModeOn = false;
}

void Map::ShowAllyAttackRange() {
	// TODO:
  if (!hoveredCharacter)
    return;

	if (hoveredCharacter->faction == Faction::kAlly)
  {
    uint32_t w = hoveredCharacter->grid_w;
    uint32_t h = hoveredCharacter->grid_h;
		// If the hovered character is not on the grid.
    if (!grid->IsGameObjectAt(hoveredCharacter, w, h))
      return;

    switch (hoveredCharacter->type)
    {
    case kBrawler:
    case kSlasher: {
      if (hoveredCharacter->isTargetInRange)
      {
        auto [w_offset, h_offset] = hoveredCharacter->GetGridFrontDirection();

				w_offset *= hoveredCharacter->distanceToTarget;
        h_offset *= hoveredCharacter->distanceToTarget;

        CellObject* cell = grid->GetCellObjectAt(w + w_offset, h + h_offset);
        cell->SetCellType(CellType_Red);
        cell->SetVisible();
			}
		}
    break;
    case kGunman: {


    }
    break;
    }
	
	
	}
}

void Map::TriggerAction()
{
  isActionTriggered = true;
	
	// Record the roster.
  for (Character* enemy : enemies)
  {
    CharacterInfo info{
			enemy->faction,
			enemy->type,
			enemy->dir,
			enemy->grid_w,
			enemy->grid_h
    };
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
    DeleteCharacterFromMap(enemy);
    enemy->Destroy();
  }

  for (Character* ally : allies)
  {
    DeleteCharacterFromMap(ally);
    ally->Destroy();
  }

  for (Character* civilian : civilians)
  {
    DeleteCharacterFromMap(civilian);
    civilian->Destroy();
  }

  grid->ClearGrid();

	enemies.clear();
  allies.clear();
  civilians.clear();

  isActionTriggered = false;
  isPlacementModeOn = false;
  placeholder = nullptr;
  isAnySelected = false;
  hoveredCharacter = nullptr;
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
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerInactiveIndicatorTextureHandle);
    }
    brawler->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerActiveIndicatorTextureHandle);
    }
    brawler->BindActiveIndicator(activeIndicator);

    // Properties
    brawler->SetFaction(kEnemy);
    brawler->SetGridLocation(w, h);
    brawler->SetDirection(dir);
    enemies.push_back(brawler);

    AddChildGameObject(brawler);
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
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanInactiveIndicatorTextureHandle);
    }
    gunman->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanActiveIndicatorTextureHandle);
    }
    gunman->BindActiveIndicator(activeIndicator);

    gunman->SetFaction(kEnemy);
    gunman->SetGridLocation(w, h);
    gunman->SetDirection(dir);
    enemies.push_back(gunman);

    AddChildGameObject(gunman);
    } break;
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
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerInactiveIndicatorTextureHandle);
    }
    brawler->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(brawlerActiveIndicatorTextureHandle);
    }
    brawler->BindActiveIndicator(activeIndicator);

    // Properties
    brawler->SetFaction(kAlly);
    brawler->SetGridLocation(w, h);
    brawler->SetDirection(dir);
    allies.push_back(brawler);

    AddChildGameObject(brawler);
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
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(slasherInactiveIndicatorTextureHandle);
    }
    slasher->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(slasherActiveIndicatorTextureHandle);
    }
    slasher->BindActiveIndicator(activeIndicator);

    // Properties
    slasher->SetFaction(kAlly);
    slasher->SetGridLocation(w, h);
    slasher->SetDirection(dir);
    allies.push_back(slasher);

    AddChildGameObject(slasher);
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
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanInactiveIndicatorTextureHandle);
    }
    gunman->BindInactiveIndicator(inactiveIndicator);

    // Bind an active indicator.
    auto* activeIndicator = world->CreateGameObject();
    if (auto* bbComp = activeIndicator->CreateComponent<BillboardComponent>();
        bbComp)
    {
      world->_renderer->CreateBillboard(bbComp->billboard);
      bbComp->SetScale({kIndicatorScale, kIndicatorScale, kIndicatorScale});
      bbComp->SetTexture(gunmanActiveIndicatorTextureHandle);
    }
    gunman->BindActiveIndicator(activeIndicator);

    gunman->SetFaction(kAlly);
    gunman->SetGridLocation(w, h);
    gunman->SetDirection(dir);
    allies.push_back(gunman);

    AddChildGameObject(gunman);
  }
  break;
  }
}

void Map::CreateCivillianAt(uint32_t w, uint32_t h, Direction dir)
{
	// TODO: Civilian model
  Civilian* civilian =
      world->CreateGameObjectFromModel<Civilian>(civilianModelHandle);
  civilian->SetGridLocation(w, h);
  civilian->SetDirection(dir);
  civilians.push_back(civilian);
}

void Map::CreateObstacleAt(uint32_t w, uint32_t h) {
  // TODO:
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

void Map::OnAwake() {
  // Translate({-4.f, 0.f, -4.f});
}

void Map::Update(float dt) {
  // Rotate this map.
  /*if (INPUT.IsKeyPress(Key::Q))
  {
    parent->RotateAroundYAxis(dt);
  }
  if (INPUT.IsKeyPress(Key::E))
  {
    parent->RotateAroundYAxis(-dt);
  }*/

	// Action mode
	if (isActionTriggered)
  {
		// TODO: Remove a simulating character.

    if (INPUT.IsKeyPress(Key::R))
    {
      ResetGame();
    }
	}
	// Placement mode
	else if (isPlacementModeOn)
  {
		// If a placeholder is not set
    if (!placeholder)
    {
      isPlacementModeOn = false;
		}

		TranslatePlaceholder();
    
		// Cancel placement mode.
		if (INPUT.IsKeyDown(Key::Escape))
    {
      TurnOffPlacementMode();
      return;
		}

		// Change the direction of the placeholder.
    if (INPUT.IsKeyDown(Key::Tab))
    {
      uint32_t dir = placeholder->GetDirection();
      placeholder->SetDirection((Direction)((dir + 1) % kNumDirections));
		}

		// Place the character.
		if (INPUT.IsKeyDown(MouseState::LB))
    {
      if (grid->selectedCell)
      {
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
    if (hoveredCharacter)
    {
			// TODO: Show the distance and range 
			ShowAllyAttackRange();

			// Right Click
      if (INPUT.IsKeyDown(MouseState::RB))
      {
        // Remove the character
        if (hoveredCharacter->faction == Faction::kAlly)
        {
          DeleteCharacterFromMap(hoveredCharacter);
          hoveredCharacter->Destroy();
        } 
				else if (hoveredCharacter->faction == Faction::kEnemy)
        {
					// Register the target to be assassinated,
					// only if in assassination mode.
          if (isAssassinationMode)
          {
            assassinationTarget = hoveredCharacter;
            isAssassinationMode = false;
					}
				}
      } 
			// Left click
			else if (INPUT.IsKeyDown(MouseState::LB))
      {
        // Character selection -> Placement mode.
        // Find if any "ally character" is selected.
        if (hoveredCharacter->faction == Faction::kAlly)
        {
          // Check the type of the character.
          CharacterType type = hoveredCharacter->type;

          // Check the direction of the character.
          Direction dir = hoveredCharacter->dir;

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

			hoveredCharacter = nullptr;
    }
    else
    {
			// TODO: Turn off the mode.
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

}

void Map::PostUpdate(float dt) {
  // hoveredCharacter = nullptr;
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

void Map::TranslatePlaceholder() {
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

