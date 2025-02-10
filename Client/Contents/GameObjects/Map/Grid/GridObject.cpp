#include "GridObject.h"

#include "GameFramework/World/World.h"

#include "Core/Input/InputSystem.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

void GridObject::CreateGrid(uint32_t width, uint32_t height, float actualCellSize)
{
  this->actualCellSize = actualCellSize;
  this->width = width;
  this->height = height;

  uint32_t len = width * height;

	placements.resize(len);
  
	grid.resize(len);
	for (int i = 0; i < len; ++i)
  {
    CellObject* cell = world->CreateGameObject<CellObject>();
		AddChildGameObject(cell);

		float pos_x = actualCellSize * (i % width) + actualCellSize / 2;
    float pos_z = actualCellSize * (i / height) + actualCellSize / 2;

		// cell->SetCellType((CellType) ((i + (i / height)) % 2));
		// cell->SetScaling(1.0);
		cell->SetTranslation(pos_x, 0, pos_z);
    cell->grid = this;
    cell->SetCellPosition(i % width, i / height);

    grid[i] = cell;
	}
}

CellObject* GridObject::GetCellObjectAt(uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return nullptr;

  return grid[idx(w, h)];
}

bool GridObject::PlaceGameObjectAt(GameObject* object, uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return false;

  if (placements[idx(w, h)])
  {
    return false;
  }

	AddChildGameObject(object);
  placements[idx(w, h)] = object;
  grid[idx(w, h)]->isOccupied = true;
  return true;
}

void GridObject::ReplaceGameObjectAt(GameObject* object, uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return;

	if (placements[idx(w, h)])
  {
    RemoveChildGameObject(placements[idx(w, h)]);
    placements[idx(w, h)] = nullptr;
	}

	AddChildGameObject(object);
  placements[idx(w, h)] = object;
  grid[idx(w, h)]->isOccupied = true;
}

void GridObject::RemoveGameObject(GameObject* object)
{
  uint32_t i{0};
  for (auto& p : placements)
  {
    if (p == object)
    {
      RemoveChildGameObject(object);
      p = nullptr;
      grid[i]->isOccupied = false;
      return;
    }
    ++i;
  }
}

bool GridObject::MoveGameObjectTo(GameObject* object, uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return false;

	if (GetGameObjectAt(w, h) != nullptr)
  {
    return false;
  }
  else
  {
    RemoveGameObject(object);
    PlaceGameObjectAt(object, w, h);
    return true;
	}
}

GameObject* GridObject::GetGameObjectAt(uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return nullptr;

  return placements[idx(w, h)];
}

bool GridObject::IsGameObjectAt(GameObject* object, uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    return false;

  return GetGameObjectAt(w, h) == object;
}

std::pair<float, float> GridObject::GetActualPositionAt(uint32_t w, uint32_t h)
{
  if (w >= width || h >= height)
    throw std::runtime_error("grid cell out of bounds!");

  float pos_x = actualCellSize * w + actualCellSize / 2;
  float pos_z = actualCellSize * h + actualCellSize / 2;

	// Apply global transformation
  // XMVECTOR pos{pos_x, 0.f, pos_z, 1.f};
  // pos = XMVector3Transform(pos, transform->GetGlobalTransform());
	
  // return {XMVectorGetX(pos), XMVectorGetZ(pos)};
  return {pos_x, pos_z};
}

void GridObject::ClearGrid() {
  for (auto& p : placements)
  {
    if (p)
    {
      RemoveChildGameObject(p);
      p = nullptr;
    }
	}

	TurnOffSelectionMode();
}

void GridObject::TurnOnSelectionMode() {
  if (isSelectionMode)
    return;

  for (uint32_t i = 0; i < grid.size(); ++i)
  {
    grid[i]->SetCellType(CellType_Default);
    grid[i]->SetVisible();
  }

  isSelectionMode = true;
}

void GridObject::TurnOffSelectionMode() {
  if (!isSelectionMode)
    return;

	for (auto* cell : grid)
  {
    cell->SetCellType(CellType_Default);
    cell->SetInvisible();
  }

	selectedCell = nullptr;
  isSelectionMode = false;
}

void GridObject::TurnOnGridHover() {
  if (isGridHoverTurnedOn)
    return;

  isGridHoverTurnedOn = true;
}

void GridObject::TurnOffGridHover() {
  if (!isGridHoverTurnedOn)
    return;

  isGridHoverTurnedOn = false;

  for (auto* cell : grid)
  {
    cell->SetCellType(CellType_Default);
  }

  selectedCell = nullptr;
}

void GridObject::ResetCellTypes() {
  for (uint32_t i = 0; i < grid.size(); ++i)
  {
    grid[i]->SetCellType(CellType_Default);
  }
}

void GridObject::FindHoveredCell()
{
  if (!isGridHoverTurnedOn)
    return;

  bool anyHover{false};

  Vector2 mousePos{(float)INPUT.GetCurrMouseState().x,
                   (float)INPUT.GetCurrMouseState().y};
  Ray cursorRay = world->GetScreenCursorRay(mousePos);
  float t;
  for (auto* cell : grid)
  {
    BoundingOrientedBox obb;
    cell->obb.Transform(obb, XMMatrixScaling(1.3f, 0.f, 1.3f) * cell->transform->GetGlobalTransform());
    if (!cell->isOccupied && !anyHover &&
        obb.Intersects(cursorRay.position, cursorRay.direction, t))
    {
      if (selectedCell != cell)
      {
        SoundManager::PlaySound(SoundList::Grid_Placement_Hover);
      }
      cell->SetCellType(CellType_Placement);
      selectedCell = cell;
      anyHover = true;
    }
    else
    {
      cell->SetCellType(CellType_Default);
    }
  }

	if (!anyHover)
		selectedCell = nullptr;
}

void GridObject::OnAwake() {
  if (isSelectionMode)
  {
    TurnOnSelectionMode();
	}
	else
	{
    TurnOffSelectionMode();
	}
}

void GridObject::Update(float dt) {
  if (isGridHoverTurnedOn)
  {
		FindHoveredCell();
  }
  else
  {
		
	}
}
