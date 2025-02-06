#include "CellObject.h"

#include "GameFramework/World/World.h"

CellObject::CellObject(World* world) : GameObject(world)
{
  redCellModelHandle = LoadModel("Models\\Grid\\CharOver\\Grid_CharOver.glb");
  greenCellModelHandle = LoadModel("Models\\Grid\\Normal\\Grid_Normal.glb");

  rangeCellModelHandle = LoadModel("Models\\Grid\\Empty\\Grid_Empty.glb");
  damageCellModelHandle = LoadModel("Models\\Grid\\Active\\Grid_Active.glb");

  // Create cell models.

	redCell = world->CreateGameObjectFromModel(redCellModelHandle);
  greenCell = world->CreateGameObjectFromModel(greenCellModelHandle);

  rangeCell = world->CreateGameObjectFromModel(rangeCellModelHandle);
  damageCell = world->CreateGameObjectFromModel(damageCellModelHandle);
  rangeCell->SetInvisible();
  damageCell->SetInvisible();

  AddChildGameObject(redCell);
  AddChildGameObject(greenCell);

  AddChildGameObject(rangeCell);
  AddChildGameObject(damageCell);
}

CellObject::~CellObject() {
  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);
  UnloadModel(rangeCellModelHandle);
  UnloadModel(damageCellModelHandle);
}

void CellObject::SetInvisible() {
  /*auto* redMeshComp = redCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = greenCell->GetComponent<MeshComponent>();
  if (redMeshComp)
  {
    redMeshComp->SetVisible(false);
	}
  if (greenMeshComp)
  {
    greenMeshComp->SetVisible(false);
	}*/

  redCell->SetInvisible();
  greenCell->SetInvisible();

  isVisible = false;
}

void CellObject::SetVisible() {
  /*auto* redMeshComp = redCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = greenCell->GetComponent<MeshComponent>();
  if (redMeshComp)
  {
    redMeshComp->SetVisible(true);
  }
  if (greenMeshComp)
  {
    greenMeshComp->SetVisible(true);
  }*/

  isVisible = true;
}

void CellObject::ClearZone() {
  rangeCell->SetInvisible();
  damageCell->SetInvisible();
}

void CellObject::SetRangeZone() {
  rangeCell->SetVisible();
  damageCell->SetInvisible();
}

void CellObject::SetDamageZone() {
  rangeCell->SetInvisible();
  damageCell->SetVisible();
}

void CellObject::SetCellType(CellType type) {
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

void CellObject::OnAwake()
{

}

void CellObject::Update(float dt) {

  if (isVisible)
  {
    switch (type)
    {
    case CellType_Green: {
      redCell->SetInvisible();
      greenCell->SetVisible();
    }
    break;
    case CellType_Red: {
      redCell->SetVisible();
      greenCell->SetInvisible();
    }
    break;
    }
  }
}
