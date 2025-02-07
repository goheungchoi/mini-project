#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class AgentStorage : public UIPanel
{
public:
  AgentStorage(class World* world);
  virtual ~AgentStorage();

  void Update(float dt) override;

private:
  class UIButton* _fistAgentBtn{nullptr};
  class UIImage* _fistAgentImgs[2]{nullptr};
  bool bFistUseFlag = false;

  class UIButton* _gunAgentBtn{nullptr};
  class UIImage* _gunAgentImgs[2]{nullptr};
  bool bGunUseFlag = false;

  class UIButton* _rushAgentBtn{nullptr};
  class UIImage* _rushAgentImgs[2]{nullptr};
  bool bRushUseFlag = false;

};