#include "CellObject.h"

#include "GameFramework/World/World.h"

CellObject::CellObject(World* world) : GameObject(world)
{
  defaultCellModelHandle = LoadModel("Models\\Grid\\Normal\\Grid_Normal.glb");
  placementCellModelHandle =
      LoadModel("Models\\Grid\\CharOver\\Grid_CharOver.glb");
  rangeCellModelHandle = LoadModel("Models\\Grid\\Empty\\Grid_Empty.glb");
  damageCellModelHandle = LoadModel("Models\\Grid\\Active\\Grid_Active.glb");

  // Create cell models.

  defaultCell = world->CreateGameObjectFromModel(defaultCellModelHandle);
  placementCell = world->CreateGameObjectFromModel(placementCellModelHandle);

  rangeCell = world->CreateGameObjectFromModel(rangeCellModelHandle);
  damageCell = world->CreateGameObjectFromModel(damageCellModelHandle);

  AddChildGameObject(defaultCell);
  AddChildGameObject(placementCell);
  AddChildGameObject(rangeCell);
  AddChildGameObject(damageCell);
}

CellObject::~CellObject()
{
  UnloadModel(defaultCellModelHandle);
  UnloadModel(placementCellModelHandle);
  UnloadModel(rangeCellModelHandle);
  UnloadModel(damageCellModelHandle);
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

void CellObject::OnAwake() {}

void CellObject::Update(float dt)
{
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
    case CellType_DamageZone:
      damageCell->SetVisible();
      break;
    }
  }
}
