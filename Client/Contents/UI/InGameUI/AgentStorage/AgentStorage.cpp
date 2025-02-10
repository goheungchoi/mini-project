#include "AgentStorage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

int Agent::numGunAgent = 0;
int Agent::prevAgentNum = 0;

Agent::Agent(World* world, CharacterType charType, Vector2 pos) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _charType = charType;

  switch (charType)
  {
  case kBrawler:
    {
      _AgentImgs[0] = CreateUI<UIImage>(L"FistAgent_Act");
      _AgentImgs[1] = CreateUI<UIImage>(L"FistAgent_Deact");
      _AgentImgs[2] = CreateUI<UIImage>(L"FistAgent_Hover");

      _AgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png", pos);
      _AgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Fist.png", pos);
      _AgentImgs[2]->SetSprite("2D\\UI\\UI_Storage_Hover_Fist.png", pos);

      _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);

      _AgentBtn = CreateUI<UIButton>(L"FistAgentBtn");
      _AgentBtn->SetSize(_AgentImgs[0]->GetSize());
      _AgentBtn->SetCenterPos(pos);

#ifdef _DEBUG
      _AgentBtn->SetDebugDraw(true);
#endif // _DEBUG

      _AgentBtn->AddOnClickHandler([this]() {
        if (_map && bUseFlag == false)
        {
          _map->TurnOnPlacementMode(kBrawler, kNorth);
          bUseFlag = true;
        }
      });

      _AgentBtn->AddOnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Active);
      });

      _AgentBtn->AddOnUnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      });

      break;
    }

  case kSlasher:
    // RushAgent
    {
      _AgentImgs[0] = CreateUI<UIImage>(L"RushAgent_Act");
      _AgentImgs[1] = CreateUI<UIImage>(L"RushAgent_Deact");
      _AgentImgs[2] = CreateUI<UIImage>(L"RushAgent_Hover");

      _AgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Rush.png", pos);
      _AgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Rush.png", pos);
      _AgentImgs[2]->SetSprite("2D\\UI\\UI_Storage_Hover_Rush.png", pos);

      _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);

      _AgentBtn = CreateUI<UIButton>(L"RushAgentBtn");
      _AgentBtn->SetSize(_AgentImgs[0]->GetSize());
      _AgentBtn->SetCenterPos(pos);

#ifdef _DEBUG
      _AgentBtn->SetDebugDraw(true);
#endif // _DEBUG

      _AgentBtn->AddOnClickHandler([this]() {
        if (_map && bUseFlag == false)
        {
          _map->TurnOnPlacementMode(kSlasher, kNorth);
          bUseFlag = true;
        }
      });

      _AgentBtn->AddOnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Active);
      });

      _AgentBtn->AddOnUnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      });

      break;
    }

  case kGunman:
    // GunAgent
    {
      std::wstring wNumAgent = std::to_wstring(numGunAgent);

      _AgentImgs[0] = CreateUI<UIImage>(L"GunAgent_Act" + wNumAgent);
      _AgentImgs[1] = CreateUI<UIImage>(L"GunAgent_Deact" + wNumAgent);
      _AgentImgs[2] = CreateUI<UIImage>(L"GunAgent_Hover" + wNumAgent);

      _AgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Gun.png", pos);
      _AgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Gun.png", pos);
      _AgentImgs[2]->SetSprite("2D\\UI\\UI_Storage_Hover_Gun.png", pos);

      _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);

      _AgentBtn = CreateUI<UIButton>(L"GunAgentBtn" + wNumAgent);
      _AgentBtn->SetSize(_AgentImgs[0]->GetSize());
      _AgentBtn->SetCenterPos(pos);

#ifdef _DEBUG
      _AgentBtn->SetDebugDraw(true);
#endif // _DEBUG

      _AgentBtn->AddOnClickHandler([this]() {
        if (_map && bUseFlag == false)
        {
          _map->TurnOnPlacementMode(kGunman, kNorth);
          bUseFlag = true;
        }
      });

      _AgentBtn->AddOnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Active);
      });

      _AgentBtn->AddOnUnHoveredHandler([this]() {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      });

      numGunAgent++;

      break;
    }

  default:
    break;
  }
}

Agent::~Agent() {}

void Agent::Update(float dt)
{
  __super::Update(dt);

  if (bUseFlag == true)
  {
    _AgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _AgentImgs[1]->SetStatus(EStatus::EStatus_Active);
    _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  }

  if (_map->deleteCharType == _charType)
  {
    if (numGunAgent < 2)
    {
      bUseFlag = false;
      _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
      _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
    }
    else if (numGunAgent >= 2)
    {
      if (_map->deleteCharType != CharacterType::kGunman)
      {
        bUseFlag = false;
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      }
      else
      {
        auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                                 ->GetUI<AgentStorage>(L"AgentStorage");

        if (agentStorage)
        {
          if (agentStorage->GetUI<Agent>(L"Agent2")->bUseFlag == false)
          {
            agentStorage->GetUI<Agent>(L"Agent3")->bUseFlag = false;
            agentStorage->GetUI<Agent>(L"Agent3")->_AgentImgs[0]->SetStatus(
                EStatus::EStatus_Active);
            agentStorage->GetUI<Agent>(L"Agent3")->_AgentImgs[1]->SetStatus(
                EStatus::EStatus_Inactive);
          }
          else if (agentStorage->GetUI<Agent>(L"Agent2")->bUseFlag == true)
          {
            agentStorage->GetUI<Agent>(L"Agent2")->bUseFlag = false;
            agentStorage->GetUI<Agent>(L"Agent2")->_AgentImgs[0]->SetStatus(
                EStatus::EStatus_Active);
            agentStorage->GetUI<Agent>(L"Agent2")->_AgentImgs[1]->SetStatus(
                EStatus::EStatus_Inactive);
          }
        }
      }
    }

    if (!_map->isPlacementModeOn)
    {
      if (prevAgentNum == _map->GetNumAllies())
      {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      }
      else
      {
        _AgentImgs[0]->SetStatus(EStatus::EStatus_Active);
        _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
        _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);
      }
    }

    _map->deleteCharType = CharacterType::kCivilian;
  }

  prevAgentNum = _map->GetNumAllies();
}

AgentStorage::AgentStorage(World* world) : UIPanel(world) {}

AgentStorage::~AgentStorage()
{
  for (auto& agent : AgentList)
  {
    agent->BeginDestroy();
    agent->Destroy();
    agent->FinishDestroy();
  }
  AgentList.clear();
}

void AgentStorage::SetAgent(CharacterType charType, Vector2 pos)
{
  std::wstring name = L"Agent" + std::to_wstring(AgentList.size());
  Agent* newAgent = CreateUI<Agent>(name, charType, pos);

  AgentList.push_back(newAgent);
}
