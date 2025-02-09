#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/GameObjects/Map/Characters/Character.h"

class Agent : public UIPanel
{
public:
  Agent(class World* world, CharacterType charType, Vector2 pos);
  ~Agent();

  void Update(float dt) override;

private:
  class UIButton* _AgentBtn{nullptr};
  class UIImage* _AgentImgs[2]{nullptr};
  bool bUseFlag = false;
  static int numAgent;
  CharacterType _charType;
};

class AgentStorage : public UIPanel
{
public:
  AgentStorage(class World* world);
  virtual ~AgentStorage();

  void SetAgent(CharacterType charType, Vector2 pos);

private:
  vector<Agent*> AgentList;
};