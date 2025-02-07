#include "AgentStorage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

AgentStorage::AgentStorage(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  // FistAgent
  {
    _fistAgentImgs[0] = CreateUI<UIImage>(L"FistAgent_Act");
    _fistAgentImgs[1] = CreateUI<UIImage>(L"FistAgent_Deact");

    _fistAgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png",
                                 {1400, 960});
    _fistAgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Fist.png",
                                 {1400, 960});
    _fistAgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);

    _fistAgentBtn = CreateUI<UIButton>(L"FistAgentBtn");
    _fistAgentBtn->SetSize(_fistAgentImgs[0]->GetSize());
    _fistAgentBtn->SetCenterPos({1400, 960});

#ifdef _DEBUG
    _fistAgentBtn->SetDebugDraw(true);
#endif // _DEBUG

    _fistAgentBtn->AddOnClickHandler([this]() {
      if (_map && bFistUseFlag == false)
      {
        _map->TurnOnPlacementMode(kBrawler, kNorth);
        bFistUseFlag = true;
      }
    });

    //_fistAgentBtn->AddOnHoveredHandler([this]() {
    //  _fistAgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    //  _fistAgentImgs[1]->SetStatus(EStatus::EStatus_Active);
    //});
  }


  // RushAgent
  {
    _rushAgentImgs[0] = CreateUI<UIImage>(L"RushAgent_Act");
    _rushAgentImgs[1] = CreateUI<UIImage>(L"RushAgent_Deact");

    _rushAgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Rush.png",
                                 {1600, 960});
    _rushAgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Rush.png",
                                 {1600, 960});
    _rushAgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);

    _rushAgentBtn = CreateUI<UIButton>(L"RushAgentBtn");
    _rushAgentBtn->SetSize(_rushAgentImgs[0]->GetSize());
    _rushAgentBtn->SetCenterPos({1600, 960});

#ifdef _DEBUG
    _rushAgentBtn->SetDebugDraw(true);
#endif // _DEBUG

    _rushAgentBtn->AddOnClickHandler([this]() {
      if (_map && bGunUseFlag == false)
      {
        _map->TurnOnPlacementMode(kSlasher, kNorth);
        bRushUseFlag = true;
      }
    });
  }


  // GunAgent
  {
    _gunAgentImgs[0] = CreateUI<UIImage>(L"GunAgent_Act");
    _gunAgentImgs[1] = CreateUI<UIImage>(L"GunAgent_Deact");

    _gunAgentImgs[0]->SetSprite("2D\\UI\\UI_Storage_Act_Gun.png", {1800, 960});
    _gunAgentImgs[1]->SetSprite("2D\\UI\\UI_Storage_Deact_Gun.png",
                                {1800, 960});
    _gunAgentImgs[1]->SetStatus(EStatus::EStatus_Inactive);

    _gunAgentBtn = CreateUI<UIButton>(L"GunAgentBtn");
    _gunAgentBtn->SetSize(_gunAgentImgs[0]->GetSize());
    _gunAgentBtn->SetCenterPos({1800, 960});

#ifdef _DEBUG
    _gunAgentBtn->SetDebugDraw(true);
#endif // _DEBUG

    _gunAgentBtn->AddOnClickHandler([this]() {
      if (_map && bGunUseFlag == false)
      {
        _map->TurnOnPlacementMode(kGunman, kNorth);
        bGunUseFlag = true;
      }
    });
  }


}

AgentStorage::~AgentStorage() {}

void AgentStorage::Update(float dt)
{
  __super::Update(dt);

  // FistAgent
  {
    if (bFistUseFlag == true)
    {
      _fistAgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
      _fistAgentImgs[1]->SetStatus(EStatus::EStatus_Active);
    }
  }

  // GunAgent
  {
    if (bGunUseFlag == true)
    {
      _gunAgentImgs[0]->SetStatus(EStatus::EStatus_Inactive);
      _gunAgentImgs[1]->SetStatus(EStatus::EStatus_Active);
    }
  }
}
