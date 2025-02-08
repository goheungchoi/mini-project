#pragma once

#include "Contents/Levels/GameLevel.h"
#include "GameFramework/World/World.h"

class Level5 : public GameLevel
{
public:
  Level5(const std::string& name) : GameLevel(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();

  void TriggerAction();
};
