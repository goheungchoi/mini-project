#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/GameObjects/Map/Characters/Character.h"

class Agent : public UIPanel
{
public:
  Agent(class World* world, CharacterType charType, Vector2 pos);
  ~Agent();

  void Update(float dt) override;

  class UIButton* _AgentBtn{nullptr};
private:
  class UIImage* _AgentImgs[3]{nullptr};
  bool bUseFlag = false;
  static int numGunAgent;
  static int prevAgentNum;
  CharacterType _charType;
};

class AgentStorage : public UIPanel
{
public:
  AgentStorage(class World* world);
  virtual ~AgentStorage();

  void SetAgent(CharacterType charType, Vector2 pos);

  vector<Agent*> AgentList;
};