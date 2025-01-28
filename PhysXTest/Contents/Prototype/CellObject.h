#pragma once

#include "GameFramework/GameObject/GameObject.h"

class CellObject : public GameObject
{
  static size_t count;

protected:
  MeshHandle redCell;
  MeshHandle greenCell;

public:
  CellObject(World* world) : GameObject(world) {
    auto redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
    const auto& redCellModel = AccessModelData(redCellModelHandle);
    if (!redCellModel.meshes.empty())
    {
      redCell = *redCellModel.meshes.begin();
		}

    auto greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");
    const auto& greenCellModel = AccessModelData(greenCellModelHandle);
    if (!greenCellModel.meshes.empty())
    {
      greenCell = *greenCellModel.meshes.begin();
    }

    // Create a mesh component
    MeshComponent* cell = CreateComponent<MeshComponent>();
    cell->AddSubMesh(count % 2 == 0 ? greenCell : redCell);
    cell->RegisterMeshToWorld();

		count++;
	}

  // Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  virtual void OnAwake() {}
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}
};
