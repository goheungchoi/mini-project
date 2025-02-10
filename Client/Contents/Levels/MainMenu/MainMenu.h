#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"


class MainMenu : public Level
{
  class TransitionUI* transitionUI;

public:
  MainMenu(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;

  void SetEndingFlag(int i);

protected:
  bool ending1 = false;
  bool ending2 = false;
  class MainMenuUI* mainmenuUI;
  virtual void CreateMap();

  void TriggerAction();
};
