#include "CellObject.h"

#include "GameFramework/World/World.h"

CellObject::CellObject(World* world) : GameObject(world)
{
  defaultCellModelHandle = LoadModel("Models\\Grid\\Normal\\Grid_Normal.glb");
  placementCellModelHandle =
      LoadModel("Models\\Grid\\CharOver\\Grid_CharOver.glb");
  rangeCellModelHandle = LoadModel("Models\\Grid\\Empty\\Grid_Empty.glb");
  dashCellModelHandle = LoadModel("Models\\Grid\\Dash\\Grid_Dir.glb");
  damageCellModelHandle = LoadModel("Models\\Grid\\Active\\Grid_Active.glb");

  // Create cell models.

  defaultCell = world->CreateGameObjectFromModel(defaultCellModelHandle);
  placementCell = world->CreateGameObjectFromModel(placementCellModelHandle);

  rangeCell = world->CreateGameObjectFromModel(rangeCellModelHandle);
  dashCell = world->CreateGameObjectFromModel(dashCellModelHandle);
  damageCell = world->CreateGameObjectFromModel(damageCellModelHandle);

  AddChildGameObject(defaultCell);
  AddChildGameObject(placementCell);
  AddChildGameObject(rangeCell);
  AddChildGameObject(dashCell);
  AddChildGameObject(damageCell);
}

CellObject::~CellObject()
{
  UnloadModel(damageCellModelHandle);
  UnloadModel(dashCellModelHandle);
  UnloadModel(rangeCellModelHandle);
  UnloadModel(placementCellModelHandle);
  UnloadModel(defaultCellModelHandle);
}

void CellObject::SetInvisible()
{
  ClearCell();
  isVisible = false;
}

void CellObject::SetVisible()
{
  isVisible = true;
}

void CellObject::ClearCell()
{
  defaultCell->SetInvisible();
  placementCell->SetInvisible();
  rangeCell->SetInvisible();
  dashCell->SetInvisible();
  damageCell->SetInvisible();
}

void CellObject::SetCellType(CellType type)
{
  this->type = type;
}

CellType CellObject::GetCellType()
{
  return type;
}

void CellObject::SetCellPosition(int w, int h)
{
  this->w = w;
  this->h = h;
}

std::pair<int, int> CellObject::GetCellPosition()
{
  return {w, h};
}

void CellObject::SetCellDirection(Direction dir) {
  this->dir = dir;
  bDirectionChanged = true;
}

Direction CellObject::GetCellDirection()
{
  return dir;
}

void CellObject::OnAwake() {
  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }
}

void CellObject::Update(float dt)
{
  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }

  if (isVisible)
  {
    ClearCell();
    switch (type)
    {
    case CellType_Default:
      if (isOccupied)
        defaultCell->SetInvisible();
      else
        defaultCell->SetVisible();
      break;
    case CellType_Placement:
      if (isOccupied)
        placementCell->SetInvisible();
      else
        placementCell->SetVisible();
      break;
    case CellType_RangeZone:
      rangeCell->SetVisible();
      break;
    case CellType_DashZone:
      dashCell->SetVisible();
      break;
    case CellType_DamageZone:
      damageCell->SetVisible();
      break;
    }
  }
}

void CellObject::ApplyChangedDirection() {
  switch (dir)
  {
  case kNorth:
    SetRotationAroundYAxis(XM_PI + XM_PI);
    break;
  case kEast:
    SetRotationAroundYAxis(XM_PI + XM_PIDIV2 + XM_PI);
    break;
  case kSouth:
    SetRotationAroundYAxis(XM_PI);
    break;
  case kWest:
    SetRotationAroundYAxis(XM_PIDIV2 + XM_PI);
    break;
  }

  bDirectionChanged = false;
}
