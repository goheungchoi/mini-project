#include "AgentStorage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
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
// Image 설정
      _AgentImgs[0] = CreateUI<UIImage>(L"FistAgent_Act");
      _AgentImgs[1] = CreateUI<UIImage>(L"FistAgent_Deact");
      _AgentImgs[2] = CreateUI<UIImage>(L"FistAgent_Hover");

      _AgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png", pos);
      _AgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Fist.png", pos);
      _AgentImgs[2]->SetSprite("2D\\UI\\UI_Storage_Hover_Fist.png", pos);

      _AgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _AgentImgs[2]->SetStatus(EStatus::EStatus_Inactive);

// Button 설정
      _AgentBtn = CreateUI<UIButton>(L"FistAgentBtn");
      _AgentBtn->SetSize(_AgentImgs[0]->GetSize());
      _AgentBtn->SetCenterPos(pos);

#ifdef _DEBUG
      _AgentBtn->SetDebugDraw(true);
#endif // _DEBUG

      // 버튼 클릭하면
      _AgentBtn->AddOnClickHandler([this]() {
        if (_map && bUseFlag == false)
        {
          // Brawler 배치모드 켜짐
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
}


AgentStorage::AgentStorage(World* world) : UIPanel(world)
{
  {
    {

      BrawlerDefaultImage = CreateUI<UIImage>(L"BrawlerDefaultImage");
      BrawlerHoveredImage = CreateUI<UIImage>(L"BrawlerHoveredImage");
      BrawlerInactiveImage = CreateUI<UIImage>(L"BrawlerInactiveImage");
      BrawlerButton = CreateUI<UIButton>(L"BrawlerButton");
      BrawlerDefaultImage->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png");
      BrawlerHoveredImage->SetSprite("2D\\UI\\UI_Storage_Hover_Fist.png");
      BrawlerInactiveImage->SetSprite("2D\\UI\\UI_Storage_Deact_Fist.png");
      BrawlerHoveredImage->SetOpacity(0.f);
      BrawlerInactiveImage->SetOpacity(0.f);
      BrawlerButton->SetSize(BrawlerDefaultImage->GetTextureSize());
      BrawlerButton->AddOnHoveredHandler([=]() {
        if (!IsBrawlerUsing)
        {
          BrawlerDefaultImage->SetOpacity(0.f);
          BrawlerHoveredImage->SetOpacity(1.f);
          BrawlerInactiveImage->SetOpacity(0.f);
        }
        else
        {
          BrawlerDefaultImage->SetOpacity(0.f);
          BrawlerHoveredImage->SetOpacity(0.f);
          BrawlerInactiveImage->SetOpacity(1.f);
        }
      });
      BrawlerButton->AddOnUnHoveredHandler([=]() {
        if (!IsBrawlerUsing)
        {
          BrawlerDefaultImage->SetOpacity(1.f);
          BrawlerHoveredImage->SetOpacity(0.f);
          BrawlerInactiveImage->SetOpacity(0.f);
        }
        else
        {
          BrawlerDefaultImage->SetOpacity(0.f);
          BrawlerHoveredImage->SetOpacity(0.f);
          BrawlerInactiveImage->SetOpacity(1.f);
        }
      });
    }
    {
      SlasherDefaultImage = CreateUI<UIImage>(L"SlasherDefaultImage");
      SlasherHoveredImage = CreateUI<UIImage>(L"SlasherHoveredImage");
      SlasherInactiveImage = CreateUI<UIImage>(L"SlasherInactiveImage");
      SlasherButton = CreateUI<UIButton>(L"SlasherButton");
      SlasherDefaultImage->SetSprite("2D\\UI\\UI_Storage_Act_Rush.png");
      SlasherHoveredImage->SetSprite("2D\\UI\\UI_Storage_Hover_Rush.png");
      SlasherInactiveImage->SetSprite("2D\\UI\\UI_Storage_Deact_Rush.png");
      SlasherHoveredImage->SetOpacity(0.f);
      SlasherInactiveImage->SetOpacity(0.f);
      SlasherButton->SetSize(SlasherDefaultImage->GetTextureSize());
      SlasherButton->AddOnHoveredHandler([=]() {
        if (!IsSlasherUsing)
        {
          SlasherDefaultImage->SetOpacity(0.f);
          SlasherHoveredImage->SetOpacity(1.f);
          SlasherInactiveImage->SetOpacity(0.f);
        }
        else
        {
          SlasherDefaultImage->SetOpacity(0.f);
          SlasherHoveredImage->SetOpacity(0.f);
          SlasherInactiveImage->SetOpacity(1.f);
        }
      });
      SlasherButton->AddOnUnHoveredHandler([=]() {
        if (!IsSlasherUsing)
        {
          SlasherDefaultImage->SetOpacity(1.f);
          SlasherHoveredImage->SetOpacity(0.f);
          SlasherInactiveImage->SetOpacity(0.f);
        }
        else
        {
          SlasherDefaultImage->SetOpacity(0.f);
          SlasherHoveredImage->SetOpacity(0.f);
          SlasherInactiveImage->SetOpacity(1.f);
        }
      });
    }
    {
      Gunman1DefaultImage = CreateUI<UIImage>(L"Gunman1DefaultImage");
      Gunman1HoveredImage = CreateUI<UIImage>(L"Gunman1HoveredImage");
      Gunman1InactiveImage = CreateUI<UIImage>(L"Gunman1InactiveImage");
      Gunman1Button = CreateUI<UIButton>(L"Gunman1Button");
      Gunman1DefaultImage->SetSprite("2D\\UI\\UI_Storage_Act_Gun.png");
      Gunman1HoveredImage->SetSprite("2D\\UI\\UI_Storage_Hover_Gun.png");
      Gunman1InactiveImage->SetSprite("2D\\UI\\UI_Storage_Deact_Gun.png");
      Gunman1HoveredImage->SetOpacity(0.f);
      Gunman1InactiveImage->SetOpacity(0.f);
      Gunman1Button->SetSize(Gunman1DefaultImage->GetTextureSize());
      Gunman1Button->AddOnHoveredHandler([=]() {
        if (!IsGunman1Using)
        {
          Gunman1DefaultImage->SetOpacity(0.f);
          Gunman1HoveredImage->SetOpacity(1.f);
          Gunman1InactiveImage->SetOpacity(0.f);
        }
        else
        {
          Gunman1DefaultImage->SetOpacity(0.f);
          Gunman1HoveredImage->SetOpacity(0.f);
          Gunman1InactiveImage->SetOpacity(1.f);
        }
      });
      Gunman1Button->AddOnUnHoveredHandler([=]() {
        if (!IsGunman1Using)
        {
          Gunman1DefaultImage->SetOpacity(1.f);
          Gunman1HoveredImage->SetOpacity(0.f);
          Gunman1InactiveImage->SetOpacity(0.f);
        }
        else
        {
          Gunman1DefaultImage->SetOpacity(0.f);
          Gunman1HoveredImage->SetOpacity(0.f);
          Gunman1InactiveImage->SetOpacity(1.f);
        }
      });
    }
    if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
    {
      Gunman2DefaultImage = CreateUI<UIImage>(L"Gunman2DefaultImage");
      Gunman2HoveredImage = CreateUI<UIImage>(L"Gunman2HoveredImage");
      Gunman2InactiveImage = CreateUI<UIImage>(L"Gunman2InactiveImage");
      Gunman2Button = CreateUI<UIButton>(L"Gunman2Button");
      Gunman2DefaultImage->SetSprite("2D\\UI\\UI_Storage_Act_Gun.png");
      Gunman2HoveredImage->SetSprite("2D\\UI\\UI_Storage_Hover_Gun.png");
      Gunman2InactiveImage->SetSprite("2D\\UI\\UI_Storage_Deact_Gun.png");
      Gunman2HoveredImage->SetOpacity(0.f);
      Gunman2InactiveImage->SetOpacity(0.f);
      Gunman2Button->SetSize(Gunman2DefaultImage->GetTextureSize());
      Gunman2Button->AddOnHoveredHandler([=]() {
        if (!IsGunman1Using)
        {
          Gunman2DefaultImage->SetOpacity(0.f);
          Gunman2HoveredImage->SetOpacity(1.f);
          Gunman2InactiveImage->SetOpacity(0.f);
        }
        else
        {
          Gunman2DefaultImage->SetOpacity(0.f);
          Gunman2HoveredImage->SetOpacity(0.f);
          Gunman2InactiveImage->SetOpacity(1.f);
        }
      });
      Gunman2Button->AddOnUnHoveredHandler([=]() {
        if (!IsGunman1Using)
        {
          Gunman2DefaultImage->SetOpacity(1.f);
          Gunman2HoveredImage->SetOpacity(0.f);
          Gunman2InactiveImage->SetOpacity(0.f);
        }
        else
        {
          Gunman2DefaultImage->SetOpacity(0.f);
          Gunman2HoveredImage->SetOpacity(0.f);
          Gunman2InactiveImage->SetOpacity(1.f);
        }
      });



    }
  }

  if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3) //4명
  {

    BrawlerDefaultImage->SetCenterPos({1250, 925});
    BrawlerHoveredImage->SetCenterPos({1250, 925});
    BrawlerInactiveImage->SetCenterPos({1250, 925});
    BrawlerButton->SetCenterPos({1250, 925});
    SlasherDefaultImage->SetCenterPos({1430, 925});
    SlasherHoveredImage->SetCenterPos({1430, 925});
    SlasherInactiveImage->SetCenterPos({1430, 925});
    SlasherButton->SetCenterPos({1430, 925});
    Gunman1DefaultImage->SetCenterPos({1610, 925});
    Gunman1HoveredImage->SetCenterPos({1610, 925});
    Gunman1InactiveImage->SetCenterPos({1610, 925});
    Gunman1Button->SetCenterPos({1610, 925});
    Gunman2DefaultImage->SetCenterPos({1790, 925});
    Gunman2HoveredImage->SetCenterPos({1790, 925});
    Gunman2InactiveImage->SetCenterPos({1790, 925});
    Gunman2Button->SetCenterPos({1790, 925});
  }
  else
  {
    BrawlerDefaultImage->SetCenterPos({1430, 925});
    BrawlerHoveredImage->SetCenterPos({1430, 925});
    BrawlerInactiveImage->SetCenterPos({1430, 925});
    BrawlerButton->SetCenterPos({1430, 925});
    SlasherDefaultImage->SetCenterPos({1610, 925});
    SlasherHoveredImage->SetCenterPos({1610, 925});
    SlasherInactiveImage->SetCenterPos({1610, 925});
    SlasherButton->SetCenterPos({1610, 925});
    Gunman1DefaultImage->SetCenterPos({1790, 925});
    Gunman1HoveredImage->SetCenterPos({1790, 925});
    Gunman1InactiveImage->SetCenterPos({1790, 925});
    Gunman1Button->SetCenterPos({1790, 925});
  }
  SetOnActivatedEvent([=]() {

    BrawlerDefaultImage->SetOpacity(1.f);
    BrawlerHoveredImage->SetOpacity(0.f);
    BrawlerInactiveImage->SetOpacity(0.f);
    SlasherDefaultImage->SetOpacity(1.f);
    SlasherHoveredImage->SetOpacity(0.f);
    SlasherInactiveImage->SetOpacity(0.f);
    Gunman1DefaultImage->SetOpacity(1.f);
    Gunman1HoveredImage->SetOpacity(0.f);
    Gunman1InactiveImage->SetOpacity(0.f);
    if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
    {
      Gunman2DefaultImage->SetOpacity(1.f);
      Gunman2HoveredImage->SetOpacity(0.f);
      Gunman2InactiveImage->SetOpacity(0.f);
    }
    IsBrawlerUsing = false;
    IsSlasherUsing = false;
    IsGunman1Using = false;
    IsGunman2Using = false;
    {
      if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() >
          3) // 4명
      {

        BrawlerDefaultImage->SetCenterPos({1250, 925});
        BrawlerHoveredImage->SetCenterPos({1250, 925});
        BrawlerInactiveImage->SetCenterPos({1250, 925});
        BrawlerButton->SetCenterPos({1250, 925});
        SlasherDefaultImage->SetCenterPos({1430, 925});
        SlasherHoveredImage->SetCenterPos({1430, 925});
        SlasherInactiveImage->SetCenterPos({1430, 925});
        SlasherButton->SetCenterPos({1430, 925});
        Gunman1DefaultImage->SetCenterPos({1610, 925});
        Gunman1HoveredImage->SetCenterPos({1610, 925});
        Gunman1InactiveImage->SetCenterPos({1610, 925});
        Gunman1Button->SetCenterPos({1610, 925});
        Gunman2DefaultImage->SetCenterPos({1790, 925});
        Gunman2HoveredImage->SetCenterPos({1790, 925});
        Gunman2InactiveImage->SetCenterPos({1790, 925});
        Gunman2Button->SetCenterPos({1790, 925});
      }
      else
      {
        BrawlerDefaultImage->SetCenterPos({1430, 925});
        BrawlerHoveredImage->SetCenterPos({1430, 925});
        BrawlerInactiveImage->SetCenterPos({1430, 925});
        BrawlerButton->SetCenterPos({1430, 925});
        SlasherDefaultImage->SetCenterPos({1610, 925});
        SlasherHoveredImage->SetCenterPos({1610, 925});
        SlasherInactiveImage->SetCenterPos({1610, 925});
        SlasherButton->SetCenterPos({1610, 925});
        Gunman1DefaultImage->SetCenterPos({1790, 925});
        Gunman1HoveredImage->SetCenterPos({1790, 925});
        Gunman1InactiveImage->SetCenterPos({1790, 925});
        Gunman1Button->SetCenterPos({1790, 925});
      }
    }

    BrawlerButton->AddOnClickHandler([=]() {
      IsBrawlerUsing = true;
      _map->TurnOnPlacementMode(CharacterType::kBrawler, kWest);
    });
    SlasherButton->AddOnClickHandler([=]() {
      IsSlasherUsing = true;
      _map->TurnOnPlacementMode(CharacterType::kBrawler, kWest);

    });
    Gunman1Button->AddOnClickHandler([=]() {
      IsGunman1Using = true;
      _map->TurnOnPlacementMode(CharacterType::kBrawler, kWest);
    });
    if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
      Gunman2Button->AddOnClickHandler([=]() {
        IsGunman2Using = true;
        _map->TurnOnPlacementMode(CharacterType::kBrawler, kWest);
      });



  });



}

void AgentStorage::ResetAgent()
{
  
}

