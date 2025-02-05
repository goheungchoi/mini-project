#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

class Level2 : public Level
{
public:
  Level2(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();

  void TriggerAction();
};
