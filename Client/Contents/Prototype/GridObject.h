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

  std::vector<CellObject*> grid;

public:
  GridObject(World* world) : GameObject(world) {}

	void CreateGrid(uint32_t width, uint32_t height, float actualCellSize);

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
  CellObject* GetCellObjectAt(uint32_t w, uint32_t h)
  {
    if (w >= width || h >= height)
      return nullptr;

    return grid[w + h * width];
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
