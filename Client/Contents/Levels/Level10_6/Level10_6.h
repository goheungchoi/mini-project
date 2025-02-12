#pragma once

#include "Contents/Levels/GameLevel.h"
#include "GameFramework/World/World.h"

// challenge1
class Level10_6 : public GameLevel
{
public:
  Level10_6(const std::string& name) : GameLevel(name) {}

  virtual void PrepareLevel() override;
  virtual void BeginLevel() override;
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();
};