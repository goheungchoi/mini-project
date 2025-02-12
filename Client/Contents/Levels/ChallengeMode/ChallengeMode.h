#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

class ChallengeMode : public Level
{
  class TransitionUI* transitionUI;

public:
  ChallengeMode(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;


protected:
  class ChallengeModeUI* challengeModeUI;
  virtual void CreateMap();
  
  void TriggerAction();
};
