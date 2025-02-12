#pragma once
#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

class EndingCredit : public Level
{

public:
  EndingCredit(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  virtual void DestroyLevel() override;
  virtual void CleanupLevel() override;


protected:
  virtual void CreateMap();
  void TriggerAction();

private:
  class TransitionUI* _transitionUI{nullptr};
  class CreditUI* _creditUI{nullptr};

};
