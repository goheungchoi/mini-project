#include "CellObject.h"

#include "GameFramework/World/World.h"

CellObject::CellObject(World* world) : GameObject(world)
{
  redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  /*const auto& redCellModel = AccessModelData(redCellModelHandle);
  if (!redCellModel.meshes.empty())
  {
    redCell = *redCellModel.meshes.begin();
  }*/

  greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");
  /*const auto& greenCellModel = AccessModelData(greenCellModelHandle);
  if (!greenCellModel.meshes.empty())
  {
    greenCell = *greenCellModel.meshes.begin();
  }*/

  // Create a mesh component
  // cell = CreateComponent<MeshComponent>();

	redCell = world->CreateGameObjectFromModel(redCellModelHandle);
  greenCell = world->CreateGameObjectFromModel(greenCellModelHandle);
  AddChildGameObject(redCell);
  AddChildGameObject(greenCell);
}

CellObject::~CellObject() {
  UnloadModel(redCellModelHandle);
  UnloadModel(greenCellModelHandle);
}

void CellObject::SetInvisible() {
  auto* redMeshComp = redCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = greenCell->GetComponent<MeshComponent>();
  if (redMeshComp)
  {
    redMeshComp->SetVisible(false);
	}
  if (greenMeshComp)
  {
    greenMeshComp->SetVisible(false);
	}
}

void CellObject::SetVisible() {
  auto* redMeshComp = redCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = greenCell->GetComponent<MeshComponent>();
  if (redMeshComp)
  {
    redMeshComp->SetVisible(true);
  }
  if (greenMeshComp)
  {
    greenMeshComp->SetVisible(true);
  }
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
  switch (type)
  {
  case CellType_Green: {
    redCell->Activate();
    greenCell->Deactivate();
  }
  break;
  case CellType_Red: {
    redCell->Deactivate();
    greenCell->Activate();
  }
  break;
  }
}

void CellObject::Update(float dt) {
  switch (type)
  {
  case CellType_Green: {
    redCell->Activate();
    greenCell->Deactivate();
  }
  break;
  case CellType_Red: {
    redCell->Deactivate();
    greenCell->Activate();
  }
  break;
  }
}
