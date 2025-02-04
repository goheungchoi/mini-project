#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "Contents/GameObjects/CameraObject.h"

class GameLevel : public Level
{
protected:
  // Resource handles
  ModelHandle redCellModelHandle;
  ModelHandle greenCellModelHandle;

  ModelHandle mapMeshHandle;

  ModelHandle animTestHandle;
  ModelHandle handGunHandle;

  CameraObject* testCamera;

  GameObject* pivot{nullptr};
  class Map* map{nullptr};

public:
  GameLevel(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();

  void TriggerAction();
};
