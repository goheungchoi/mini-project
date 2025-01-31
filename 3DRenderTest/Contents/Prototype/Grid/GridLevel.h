#pragma once

#include "Contents/GameObjects/TestGameObject.h"
#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "GridObject.h"


////////////////////////////////////////////
//
//												(5, 5)
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
//	(0, 0)                 +x
//
//


class GridLevel : public Level
{
	using Super = Level;
protected:

  const uint32_t width, height;
  GridObject* grid{nullptr};

public:
  GridLevel(const std::string& levelName, uint32_t width, uint32_t height)
      : Level(levelName), width{width}, height{height} {}

  virtual void PrepareLevel() override
  {
    auto redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
    auto greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");
  }

  virtual void BeginLevel() override
  {
    grid = world->CreateGameObject<GridObject>();
    grid->CreateGrid(width, height, 5.f);
  }

  virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override {}
};
