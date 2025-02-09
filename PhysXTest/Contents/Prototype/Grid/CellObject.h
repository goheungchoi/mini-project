#pragma once

#include "GameFramework/GameObject/GameObject.h"

enum CellType
{
	CellType_Default,
	CellType_Placement
};

class CellObject : public GameObject
{
protected:
  CellType type{CellType_Default};

	ModelHandle placementCellModelHandle;
  ModelHandle defaultCellModelHandle;

  /*MeshHandle redCell;
  MeshHandle greenCell;*/

	// MeshComponent* cell;

	GameObject* placementCell;
  GameObject* defaultCell;

public:
  class GridObject* grid{nullptr};
  int w{-1}, h{-1};
  BoundingOrientedBox obb{{0, 0, 0}, {0.5, 0.0005f, 0.5}, {0, 0, 0, 1.f}};

  bool isVisible {false};

  CellObject(World* world);
  ~CellObject();

	void SetInvisible();
  void SetVisible();

	void SetCellType(CellType type);
  CellType GetCellType();
  void SetCellPosition(int w, int h);
  std::pair<int, int> GetCellPosition();

  // Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  void OnAwake();
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  void Update(float dt) override;
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}
};
