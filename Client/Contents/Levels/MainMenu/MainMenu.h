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

  class MainMenuUI* mainmenuUI;
protected:
  bool ending1 = false;
  bool ending2 = false;
  virtual void CreateMap();

  void TriggerAction();
};
