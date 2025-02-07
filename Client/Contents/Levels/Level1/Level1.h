#pragma once

#include "Contents/Levels/GameLevel.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "Contents/GameObjects/CameraObject.h"

class Level1 : public GameLevel
{
protected:


public:
  Level1(const std::string& name) : GameLevel(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }

protected:
  virtual void CreateMap();

  void TriggerAction();
};
