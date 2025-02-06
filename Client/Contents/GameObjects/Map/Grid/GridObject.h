#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "CellObject.h"

////////////////////////////////////////////
// 
// * How the grid looks like in 3D space.
//
//												[5, 5]
//	+z	+---+---+---+---+---+
//			| x |   |   |   | x |
//			+---+---+---+---+---+
//			|   |   |   |   |   |
//			+---+---+---+---+---+
//			|   |   |   |   |   |
//			+---+---+---+---+---+
//			|   |   |   |   |   |
//			+---+---+---+---+---+
//			| x |   |   |   | x |
//		 	+---+---+---+---+---+
//	[0, 0]                 +x
//
//


class GridObject : public GameObject
{
protected:

  float actualCellSize{0.f};
  uint32_t width{0}, height{0};

	std::vector<GameObject*> placements;

  std::vector<CellObject*> grid;

public:

	CellObject* selectedCell{nullptr};

  GridObject(World* world) : GameObject(world) {}

	void CreateGrid(uint32_t width, uint32_t height, float actualCellSize);

	uint32_t GetWidth() const { return width; }
  uint32_t GetHeight() const { return height; }

	//////////////////////////////////////////////////////////////
	//
	// * Coordinates and actual positions
  //																									[5, 5]
  //	+z	+--------------------------------------------+
  //			| (0, 4) | (1, 4) | (2, 4) | (3, 4) | (4, 4) |
  //			+--------------------------------------------+
  //			| (0, 3) | (1, 3) | (2, 3) | (3, 3) | (4, 3) |
  //			+--------------------------------------------+
  //			| (0, 2) | (1, 2) | (2, 2) | (3, 0) | (4, 0) |
  //			+--------------------------------------------+
  //			| (0, 1) | (1, 1) | (2, 1) | (3, 1) | (4, 1) |
  //			+--------------------------------------------+
  //			| (0, 0) | (1, 0) | (2, 0) | (3, 0) | (4, 0) |
  //		 	+--------------------------------------------+
  //	[0, 0]																					+x
  //
  CellObject* GetCellObjectAt(uint32_t w, uint32_t h);

	bool PlaceGameObjectAt(GameObject* object, uint32_t w, uint32_t h);

	void ReplaceGameObjectAt(GameObject* object, uint32_t w, uint32_t h);

	void RemoveGameObject(GameObject* object);

	bool MoveGameObjectTo(GameObject* object, uint32_t w, uint32_t h);

	GameObject* GetGameObjectAt(uint32_t w, uint32_t h);

	bool IsGameObjectAt(GameObject* object, uint32_t w, uint32_t h);

	std::pair<float, float> GetActualPositionAt(uint32_t w, uint32_t h);

	void ClearGrid();

	// Interaction

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;

	bool isSelectionMode{false};

  void TurnOnSelectionMode();
  void TurnOffSelectionMode();

private:

	void FindHoveredCell();

	uint32_t idx(uint32_t w, uint32_t h) { return w + h * width; }
};
