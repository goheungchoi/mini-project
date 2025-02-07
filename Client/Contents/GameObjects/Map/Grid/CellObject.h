#pragma once

#include "GameFramework/GameObject/GameObject.h"

enum CellType
{
	CellType_Default,
	CellType_Placement,
  CellType_RangeZone,
  CellType_DamageZone,
};

class CellObject : public GameObject
{
protected:
  CellType type{CellType_Default};

  ModelHandle defaultCellModelHandle;
	ModelHandle placementCellModelHandle;
  ModelHandle rangeCellModelHandle;
  ModelHandle damageCellModelHandle;

  GameObject* defaultCell;
	GameObject* placementCell;
  GameObject* rangeCell;
  GameObject* damageCell;

public:
  class GridObject* grid{nullptr};
  int w{-1}, h{-1};
  BoundingOrientedBox obb{{0, 0, 0}, {0.5, 0.0005f, 0.5}, {0, 0, 0, 1.f}};

  bool isVisible {false};

  bool isOccupied{false};

  CellObject(World* world);
  ~CellObject();

	void SetInvisible();
  void SetVisible();

  void ClearCell();
	void SetCellType(CellType type);
  CellType GetCellType();
  void SetCellPosition(int w, int h);
  std::pair<int, int> GetCellPosition();

  // Game loop events
  void OnAwake();

  void Update(float dt) override;
};
