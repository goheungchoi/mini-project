#include "Map.h"

#include "Core/Input/InputSystem.h"

#include "GameFramework/World/World.h"

#include "Contents/GameObjects/Map/Characters/Brawler/Brawler.h"
#include "Contents/GameObjects/Map/Characters/Civilian/Civilian.h"
#include "Contents/GameObjects/Map/Characters/Gunman/Gunman.h"
#include "Contents/GameObjects/Map/Characters/Slasher/Slasher.h"

Map::Map(World* world) : GameObject(world)
{
  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");

	// The base models.
	enemyModelHandle = LoadModel("Models\\Character\\Enemy\\Enemy.glb");
	playerModelHandle = LoadModel("Models\\Character\\Player\\Player.glb");
  civilianModelHandle = LoadModel("Models\\Civilian\\Animation_003.glb");

	// Set character static data.
  brawlerInactiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\BrawlerInactiveIndicator\\Indicator.glb");
  brawlerActiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\BrawlerActiveIndicator\\Indicator.glb");
  slasherInactiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\SlasherInactiveIndicator\\Indicator.glb");
  slasherActiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\SlasherActiveIndicator\\Indicator.glb");
  gunmanInactiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\GunmanInactiveIndicator\\Indicator.glb");
  gunmanActiveIndicatorModelHandle =
      LoadModel("Models\\Indicator\\GunmanActiveIndicator\\Indicator.glb");

	Character::enemyModelData = &AccessModelData(enemyModelHandle);
  Character::enemySkeletonHandle = Character::enemyModelData->skeleton;

	Character::playerModelData = &AccessModelData(playerModelHandle);
  Character::playerSkeletonHandle = Character::playerModelData->skeleton;

  Character::civilianModelData = &AccessModelData(civilianModelHandle);
  Character::civilianSkeletonHandle = Character::civilianModelData->skeleton;
  Character::civilianDeadAnimation =
      *Character::civilianModelData->animations.begin();

	auto animIt = Character::playerModelData->animations.begin();

  Character::deadAnimation = *std::next(animIt, 1);
  Character::idleAnimation = *std::next(animIt, 6);

  Character::brawlerActionAnimation = *std::next(animIt, 7);

  Character::slashReadyAnimation = *std::next(animIt, 0);
  Character::slashActionAnimation = *std::next(animIt, 5);

  Character::gunReady1Animation = *std::next(animIt, 3);
  Character::gunReady2Animation = *std::next(animIt, 4);
  Character::gunFireAnimation = *std::next(animIt, 2);


  ModelData& enemyModel = AccessModelData(enemyModelHandle);

	// Get the player model data.
	ModelData& playerModel = AccessModelData(playerModelHandle);

	// Apply different materials to the models.
	allyBrawlerModelHandle = CloneModel(playerModelHandle);
  allySlasherModelHandle = CloneModel(playerModelHandle);
  allyGunmanModelHandle = CloneModel(playerModelHandle);

	// Clone version of the model, transparent.
	clonedAllyBrawlerModelHandle = CloneModel(allyBrawlerModelHandle);
  clonedAllySlasherModelHandle = CloneModel(allySlasherModelHandle);
  clonedAllyGunmanModelHandle = CloneModel(allyGunmanModelHandle);

	ModelData& clonedAllyBrawlerModelData =
      AccessModelData(clonedAllyBrawlerModelHandle);
  for (auto matHandle : clonedAllyBrawlerModelData.materials)
  {
    MaterialData& matData = AccessMaterialData(matHandle);
    matData.alphaMode = AlphaMode::kBlend;
	}

	ModelData& clonedAllySlasherModelData =
      AccessModelData(clonedAllySlasherModelHandle);
  for (auto matHandle : clonedAllySlasherModelData.materials)
  {
    MaterialData& matData = AccessMaterialData(matHandle);
    matData.alphaMode = AlphaMode::kBlend;
  }

	ModelData& clonedAllyGunmanModelData =
      AccessModelData(clonedAllyGunmanModelHandle);
  for (auto matHandle : clonedAllyGunmanModelData.materials)
  {
    MaterialData& matData = AccessMaterialData(matHandle);
    matData.alphaMode = AlphaMode::kBlend;
  }

	// Create a grid.
	grid = world->CreateGameObject<GridObject>();
  grid->CreateGrid(6, 6, 1.4f);
  grid->Translate(-0.6f, +0.01f, -0.8f);
	AddChildGameObject(grid);
}

Map::~Map() {


  UnloadModel(playerModelHandle);
  UnloadModel(enemyModelHandle);
}

void Map::TurnOnPlacementMode(CharactorType type) {
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
    brawler->SetDirection(kNorth);
    placeholder = brawler;
  }
  break;
  case kSlasher: {
    Slasher* slasher =
        world->CreateGameObjectFromModel<Slasher>(allySlasherModelHandle);
    slasher->SetFaction(kAlly);
    slasher->SetDirection(kNorth);
    placeholder = slasher;
  }
  break;
  case kGunman: {
    Gunman* gunman =
        world->CreateGameObjectFromModel<Gunman>(allyGunmanModelHandle);
    gunman->SetFaction(kAlly);
    gunman->SetDirection(kNorth);
    placeholder = gunman;
  }
  break;
  }

	TranslatePlaceholder();

  isPlacementModeOn = true;
}

void Map::TurnOffPlacementMode() {
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

void Map::TurnOnSimulationMode() {
	// TODO:
}

void Map::TriggerAction() {
  isActionTriggered = true;

  for (Character* enemy : enemies)
  {
    enemy->TriggerAction();
	}

	for (Character* ally : allies)
  {
    ally->TriggerAction();
	}
}

void Map::ResetGame() {
	// TODO:
  isActionTriggered = false;

}

void Map::CreateEnemyAt(CharactorType type, uint32_t w, uint32_t h,
                        Direction dir)
{
  Gunman* gunman =
      world->CreateGameObjectFromModel<Gunman>(enemyModelHandle);

  // Bind indicators
  auto* inactiveIndicator =
      world->CreateGameObjectFromModel(gunmanInactiveIndicatorModelHandle);
  if (auto* meshComp = inactiveIndicator->GetComponent<MeshComponent>();
      meshComp)
  {
    meshComp->SetCastShadow(false);
  }
  inactiveIndicator->SetScaling(10.f);
  gunman->BindInactiveIndicator(inactiveIndicator);

  auto* activeIndicator =
      world->CreateGameObjectFromModel(gunmanActiveIndicatorModelHandle);
  if (auto* meshComp = activeIndicator->GetComponent<MeshComponent>(); meshComp)
  {
    meshComp->SetCastShadow(false);
  }
  activeIndicator->SetScaling(10.f);
  gunman->BindActiveIndicator(activeIndicator);

  gunman->SetFaction(kEnemy);
  gunman->SetGridLocation(w, h);
  gunman->SetDirection(dir);
  enemies.push_back(gunman);

	AddChildGameObject(gunman);
}

void Map::CreateAllyAt(CharactorType type, uint32_t w, uint32_t h, Direction dir)
{
  switch (type)
  {
  case kBrawler: {
    Brawler* brawler =
        world->CreateGameObjectFromModel<Brawler>(allyBrawlerModelHandle);

    // Bind indicators.
    auto* inactiveIndicator =
        world->CreateGameObjectFromModel(brawlerInactiveIndicatorModelHandle);
    inactiveIndicator->SetScaling(10.f);
    if (auto* meshComp = inactiveIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    brawler->BindInactiveIndicator(inactiveIndicator);

    auto* activeIndicator =
        world->CreateGameObjectFromModel(brawlerActiveIndicatorModelHandle);
    if (auto* meshComp = activeIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    activeIndicator->SetScaling(10.f);
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

    auto* inactiveIndicator =
        world->CreateGameObjectFromModel(slasherInactiveIndicatorModelHandle);
    if (auto* meshComp = inactiveIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    inactiveIndicator->SetScaling(10.f);
    slasher->BindInactiveIndicator(inactiveIndicator);

    auto* activeIndicator =
        world->CreateGameObjectFromModel(slasherActiveIndicatorModelHandle);
    if (auto* meshComp = activeIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    activeIndicator->SetScaling(10.f);
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

    auto* inactiveIndicator =
        world->CreateGameObjectFromModel(gunmanInactiveIndicatorModelHandle);
    if (auto* meshComp = inactiveIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    inactiveIndicator->SetScaling(10.f);
    gunman->BindInactiveIndicator(inactiveIndicator);

    auto* activeIndicator =
        world->CreateGameObjectFromModel(gunmanActiveIndicatorModelHandle);
    if (auto* meshComp = activeIndicator->GetComponent<MeshComponent>();
        meshComp)
    {
      meshComp->SetCastShadow(false);
    }
    activeIndicator->SetScaling(10.f);
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

void Map::CreateObstacleAt(uint32_t w, uint32_t h) {}

void Map::OnAwake() {
  // Translate({-4.f, 0.f, -4.f});
  this->Deactivate();
}

void Map::Update(float dt) {
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
  else
  {
    if (INPUT.IsKeyPress(Key::D1))
    {
      TurnOnPlacementMode(kBrawler);
      return;
    }

    if (INPUT.IsKeyPress(Key::D2))
    {
      TurnOnPlacementMode(kSlasher);
      return;
    }

    if (INPUT.IsKeyPress(Key::D3))
    {
      TurnOnPlacementMode(kGunman);
      return;
    }

    if (INPUT.IsKeyPress(Key::Space))
    {
      TriggerAction();
      return;
    }
  }
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

