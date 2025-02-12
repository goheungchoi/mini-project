#pragma once

#include "Contents/Levels/GameLevel.h"
#include "GameFramework/World/World.h"

// challenge1
class Level10_4 : public GameLevel
{
public:
  Level10_4(const std::string& name) : GameLevel(name) {}

  virtual void PrepareLevel() override;
  virtual void BeginLevel() override;
  virtual void CleanupLevel() override;

protected:
  virtual void CreateMap();
};