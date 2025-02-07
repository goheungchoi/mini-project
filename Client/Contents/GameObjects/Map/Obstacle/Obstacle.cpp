#include "Obstacle.h"

#include "GameFramework/World/World.h"

#include "Contents/GameObjects/Map/Map.h"
#include "Contents/GameObjects/Map/Grid/GridObject.h"

Obstacle::Obstacle(class World* world) : GameObject(world)
{
  SetGameObjectTag("Obstacle");
}

void Obstacle::SetObstacleType(ObstacleType type) {
  this->type = type;
}

ObstacleType Obstacle::GetObstacleType()
{
  return type;
}

void Obstacle::SetDirection(Direction direction)
{
  this->dir = direction;
  bDirectionChanged = true;
}

Direction Obstacle::GetDirection()
{
  return dir;
}

void Obstacle::SetGridLocation(uint32_t w, uint32_t h) {
  grid_w = w;
  grid_h = h;
  bGridLocationChanged = true;
}

std::pair<uint32_t, uint32_t> Obstacle::GetGridLocation()
{
  return {grid_w, grid_h};
}

std::pair<int, int> Obstacle::GetGridFrontDirection()
{
  int w_offset{0}, h_offset{0};

  switch (dir)
  {
  case kNorth:
    h_offset = 1;
    break;
  case kEast:
    w_offset = 1;
    break;
  case kSouth:
    h_offset = -1;
    break;
  case kWest:
    w_offset = -1;
    break;
  }

  return {w_offset, h_offset};
}

void Obstacle::OnAwake() {
  grid = world->FindGameObjectByType<GridObject>();
  if (!grid)
  {
    throw std::runtime_error("Can't find grid!");
  }

  map = world->FindGameObjectByType<Map>();
  if (!map)
  {
    throw std::runtime_error("Can't find map!");
  }

  if (bGridLocationChanged)
  {
    ApplyChangedGridLocation();
  }

  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }
}

void Obstacle::Update(float dt) {
  if (bGridLocationChanged)
  {
    ApplyChangedGridLocation();
  }

  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }
}

void Obstacle::ApplyChangedGridLocation() {
  // Place the game object on the grid.
  grid->MoveGameObjectTo(this, grid_w, grid_h);
  // Translate the game object.
  auto [x, z] = grid->GetActualPositionAt(grid_w, grid_h);
  SetTranslation(x, 0, z);
  // Turn off the flag.
  bGridLocationChanged = false;
}

void Obstacle::ApplyChangedDirection() {
  switch (dir)
  {
  case kNorth:
    SetRotationAroundYAxis(XM_PI);
    break;
  case kEast:
    SetRotationAroundYAxis(XM_PI + XM_PIDIV2);
    break;
  case kSouth:
    SetRotationAroundYAxis(0);
    break;
  case kWest:
    SetRotationAroundYAxis(XM_PIDIV2);
    break;
  }

  bDirectionChanged = false;
}
