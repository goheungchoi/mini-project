#include "CellObject.h"

#include "GameFramework/World/World.h"

CellObject::CellObject(World* world) : GameObject(world)
{
  placementCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
  /*const auto& redCellModel = AccessModelData(redCellModelHandle);
  if (!redCellModel.meshes.empty())
  {
    redCell = *redCellModel.meshes.begin();
  }*/

  defaultCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");
  /*const auto& greenCellModel = AccessModelData(greenCellModelHandle);
  if (!greenCellModel.meshes.empty())
  {
    greenCell = *greenCellModel.meshes.begin();
  }*/

  // Create a mesh component
  // cell = CreateComponent<MeshComponent>();

	placementCell = world->CreateGameObjectFromModel(placementCellModelHandle);
  defaultCell = world->CreateGameObjectFromModel(defaultCellModelHandle);
  AddChildGameObject(placementCell);
  AddChildGameObject(defaultCell);
}

CellObject::~CellObject() {
  UnloadModel(placementCellModelHandle);
  UnloadModel(defaultCellModelHandle);
}

void CellObject::SetInvisible() {
  auto* redMeshComp = placementCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = defaultCell->GetComponent<MeshComponent>();
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
  auto* redMeshComp = placementCell->GetComponent<MeshComponent>();
  auto* greenMeshComp = defaultCell->GetComponent<MeshComponent>();
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
  case CellType_Default: {
    placementCell->Activate();
    defaultCell->Deactivate();
  }
  break;
  case CellType_Placement: {
    placementCell->Deactivate();
    defaultCell->Activate();
  }
  break;
  }
}

void CellObject::Update(float dt) {
  switch (type)
  {
  case CellType_Default: {
    placementCell->Activate();
    defaultCell->Deactivate();
  }
  break;
  case CellType_Placement: {
    placementCell->Deactivate();
    defaultCell->Activate();
  }
  break;
  }
}
