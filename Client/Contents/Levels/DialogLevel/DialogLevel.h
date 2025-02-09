#pragma once
#include "GameFramework/World/World.h"
#include "GameFramework\Level\Level.h"
class DialogLevel : public Level
{
public:
  DialogLevel(const std::string& name) : Level(name) {}

  void PrepareLevel() override;
  void BeginLevel() override;
  void CleanupLevel() override;

private:
  //class GameManager* gameManager{nullptr};
  class DialogUI* dialogUI{nullptr};
  class TransitionUI* transitionUI;
};
