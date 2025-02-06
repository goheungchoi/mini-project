#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class AgentStorage : public UIPanel
{
public:
  AgentStorage(class World* world);
  virtual ~AgentStorage();

private:
  class UIButton* _agentStorageBtn{nullptr};
  class UIImage* _agentStorageBtnImg{nullptr};
};