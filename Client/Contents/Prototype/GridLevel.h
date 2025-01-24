#pragma once

#include "Contents/GameObjects/TestGameObject.h"
#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

class GridLevel : public Level
{


public:
  GridLevel() : Level("Grid Level") {}

  virtual void PrepareLevel() override
  {
  }

  virtual void BeginLevel() override
  {

  }

  virtual void DestroyLevel() override {}
  virtual void CleanupLevel() override {}
};
