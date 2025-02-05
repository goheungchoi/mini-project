#include "Map.h"

#include "Core/Input/InputSystem.h"

#include "GameFramework/World/World.h"

#include "Contents/Prototype/Character/Brawler.h"
#include "Contents/Prototype/Character/Civilian.h"
#include "Contents/Prototype/Character/Gunman.h"
#include "Contents/Prototype/Character/Slasher.h"

Map::Map(World* world) : GameObject(world)
{
  animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");

	grid = world->CreateGameObject<GridObject>();
  grid->CreateGrid(6, 6, 1.4f);
  grid->Translate(-0.6f, +0.01f, -0.8f);

	AddChildGameObject(grid);
}

Map::~Map() {}

void Map::TurnOnPlacementMode(CharacterType type) {
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
        world->CreateGameObjectFromModel<Brawler>(animTestHandle);
    brawler->SetFaction(kAlly);
    brawler->SetDirection(kNorth);
    placeholder = brawler;
  }
  break;
  case kSlasher: {
    Slasher* slasher =
        world->CreateGameObjectFromModel<Slasher>(animTestHandle);
    slasher->SetFaction(kAlly);
    slasher->SetDirection(kNorth);
    placeholder = slasher;
  }
  break;
  case kGunman: {
    Gunman* gunman = world->CreateGameObjectFromModel<Gunman>(animTestHandle);
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

void Map::CreateEnemyAt(uint32_t w, uint32_t h, Direction dir)
{
  Gunman* gunman = world->CreateGameObjectFromModel<Gunman>(animTestHandle);
  gunman->SetFaction(kEnemy);
  gunman->SetGridLocation(w, h);
  gunman->SetDirection(dir);
  enemies.push_back(gunman);

	AddChildGameObject(gunman);
}

void Map::CreateAllyAt(CharacterType type, uint32_t w, uint32_t h, Direction dir)
{
  switch (type)
  {
  case kBrawler: {
    Brawler* brawler =
        world->CreateGameObjectFromModel<Brawler>(animTestHandle);
    brawler->SetFaction(kAlly);
    brawler->SetGridLocation(w, h);
    brawler->SetDirection(dir);
    allies.push_back(brawler);

    AddChildGameObject(brawler);
  }
  break;
  case kSlasher: {
    Slasher* slasher =
        world->CreateGameObjectFromModel<Slasher>(animTestHandle);
    slasher->SetFaction(kAlly);
    slasher->SetGridLocation(w, h);
    slasher->SetDirection(dir);
    allies.push_back(slasher);

    AddChildGameObject(slasher);
  }
  break;
  case kGunman: {
    Gunman* gunman = world->CreateGameObjectFromModel<Gunman>(animTestHandle);
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
  Civilian* civilian =
      world->CreateGameObjectFromModel<Civilian>(animTestHandle);
  civilian->SetGridLocation(w, h);
  civilian->SetDirection(dir);
  civilians.push_back(civilian);
}

void Map::CreateObstacleAt(uint32_t w, uint32_t h) {}

void Map::OnAwake() {
  // Translate({-4.f, 0.f, -4.f});

}

void Map::Update(float dt) {
  if (INPUT.IsKeyPress(Key::Q))
  {
    parent->RotateAroundYAxis(dt);
  }
  if (INPUT.IsKeyPress(Key::E))
  {
    parent->RotateAroundYAxis(-dt);
  }

	if (isActionTriggered)
  {
    if (isActionTriggered)
    {
      if (INPUT.IsKeyPress(Key::R))
      {
        ResetGame();
      }
    }
	}
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

