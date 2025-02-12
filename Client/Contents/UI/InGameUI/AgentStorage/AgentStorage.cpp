#include "AgentStorage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

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
        if (!_bHoverFlag)
        {
          SoundManager::PlaySound(SoundList::Button_Hover);
          _bHoverFlag = true;
        }

        bool isNotSimulating = !_map->isAssassinationMode &&
                            !_map->isPlacementModeOn &&
                            !_map->isHoveredCharacterChanged;
        if (isNotSimulating)
        {
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
        }
      });
      Gunman2Button->AddOnUnHoveredHandler([=]() {
        _bHoverFlag = false;

        if (!IsGunman2Using)
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
  BrawlerDefaultImage->SetScale({0.8f, 0.8f});
  BrawlerHoveredImage->SetScale({0.8f, 0.8f});
  BrawlerInactiveImage->SetScale({0.8f, 0.8f});
  BrawlerButton->SetSize(BrawlerInactiveImage->GetTextureSize() * 0.75f);
  SlasherDefaultImage->SetScale({0.8f, 0.8f});
  SlasherHoveredImage->SetScale({0.8f, 0.8f});
  SlasherInactiveImage->SetScale({0.8f, 0.8f});
  SlasherButton->SetSize(SlasherInactiveImage->GetTextureSize() * 0.75f);
  Gunman1DefaultImage->SetScale({0.8f, 0.8f});
  Gunman1HoveredImage->SetScale({0.8f, 0.8f});
  Gunman1InactiveImage->SetScale({0.8f, 0.8f});
  Gunman1Button->SetSize(Gunman1DefaultImage->GetTextureSize() * 0.75f);
  if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
  {
    Gunman2DefaultImage->SetScale({0.8f, 0.8f});
    Gunman2HoveredImage->SetScale({0.8f, 0.8f});
    Gunman2InactiveImage->SetScale({0.8f, 0.8f});
    Gunman2Button->SetSize(Gunman1DefaultImage->GetTextureSize() * 0.75f);
  }
  if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3) //4Έν
  {

    BrawlerDefaultImage->SetCenterPos({1350, 940});
    BrawlerHoveredImage->SetCenterPos({1350, 940});
    BrawlerInactiveImage->SetCenterPos({1350, 940});
    BrawlerButton->SetCenterPos({1350, 940});
    SlasherDefaultImage->SetCenterPos({1500, 940});
    SlasherHoveredImage->SetCenterPos({1500, 940});
    SlasherInactiveImage->SetCenterPos({1500, 940});
    SlasherButton->SetCenterPos({1500, 940});
    Gunman1DefaultImage->SetCenterPos({1650, 940});
    Gunman1HoveredImage->SetCenterPos({1650, 940});
    Gunman1InactiveImage->SetCenterPos({1650, 940});
    Gunman1Button->SetCenterPos({1650, 940});
    Gunman2DefaultImage->SetCenterPos({1800, 940});
    Gunman2HoveredImage->SetCenterPos({1800, 940});
    Gunman2InactiveImage->SetCenterPos({1800, 940});
    Gunman2Button->SetCenterPos({1800, 940});
  }
  else
  {
    BrawlerDefaultImage->SetCenterPos({1500, 940});
    BrawlerHoveredImage->SetCenterPos({1500, 940});
    BrawlerInactiveImage->SetCenterPos({1500, 940});
    BrawlerButton->SetCenterPos({1500, 940});
    SlasherDefaultImage->SetCenterPos({1650, 940});
    SlasherHoveredImage->SetCenterPos({1650, 940});
    SlasherInactiveImage->SetCenterPos({1650, 940});
    SlasherButton->SetCenterPos({1650, 940});
    Gunman1DefaultImage->SetCenterPos({1800, 940});
    Gunman1HoveredImage->SetCenterPos({1800, 940});
    Gunman1InactiveImage->SetCenterPos({1800, 940});
    Gunman1Button->SetCenterPos({1800, 940});
  }
  SetOnActivatedEvent([=]() {

    //BrawlerDefaultImage->SetOpacity(1.f);
    //BrawlerHoveredImage->SetOpacity(0.f);
    //BrawlerInactiveImage->SetOpacity(0.f);
    //SlasherDefaultImage->SetOpacity(1.f);
    //SlasherHoveredImage->SetOpacity(0.f);
    //SlasherInactiveImage->SetOpacity(0.f);
    //Gunman1DefaultImage->SetOpacity(1.f);
    //Gunman1HoveredImage->SetOpacity(0.f);
    //Gunman1InactiveImage->SetOpacity(0.f);
    //if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
    //{
    //  Gunman2DefaultImage->SetOpacity(1.f);
    //  Gunman2HoveredImage->SetOpacity(0.f);
    //  Gunman2InactiveImage->SetOpacity(0.f);
    //}
    //IsBrawlerUsing = false;
    //IsSlasherUsing = false;
    //IsGunman1Using = false;
    //IsGunman2Using = false;
    //{
    //  if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() >
    //      3) // 4Έν
    //  {

    //    BrawlerDefaultImage->SetCenterPos({1250, 925});
    //    BrawlerHoveredImage->SetCenterPos({1250, 925});
    //    BrawlerInactiveImage->SetCenterPos({1250, 925});
    //    BrawlerButton->SetCenterPos({1250, 925});
    //    SlasherDefaultImage->SetCenterPos({1430, 925});
    //    SlasherHoveredImage->SetCenterPos({1430, 925});
    //    SlasherInactiveImage->SetCenterPos({1430, 925});
    //    SlasherButton->SetCenterPos({1430, 925});
    //    Gunman1DefaultImage->SetCenterPos({1610, 925});
    //    Gunman1HoveredImage->SetCenterPos({1610, 925});
    //    Gunman1InactiveImage->SetCenterPos({1610, 925});
    //    Gunman1Button->SetCenterPos({1610, 925});
    //    Gunman2DefaultImage->SetCenterPos({1790, 925});
    //    Gunman2HoveredImage->SetCenterPos({1790, 925});
    //    Gunman2InactiveImage->SetCenterPos({1790, 925});
    //    Gunman2Button->SetCenterPos({1790, 925});
    //  }
    //  else
    //  {
    //    BrawlerDefaultImage->SetCenterPos({1430, 925});
    //    BrawlerHoveredImage->SetCenterPos({1430, 925});
    //    BrawlerInactiveImage->SetCenterPos({1430, 925});
    //    BrawlerButton->SetCenterPos({1430, 925});
    //    SlasherDefaultImage->SetCenterPos({1610, 925});
    //    SlasherHoveredImage->SetCenterPos({1610, 925});
    //    SlasherInactiveImage->SetCenterPos({1610, 925});
    //    SlasherButton->SetCenterPos({1610, 925});
    //    Gunman1DefaultImage->SetCenterPos({1790, 925});
    //    Gunman1HoveredImage->SetCenterPos({1790, 925});
    //    Gunman1InactiveImage->SetCenterPos({1790, 925});
    //    Gunman1Button->SetCenterPos({1790, 925});
    //  }
    //}


  });


#ifdef _DEBUG
  BrawlerButton->SetDebugDraw(true);
  SlasherButton->SetDebugDraw(true);
  Gunman1Button->SetDebugDraw(true);
  if (static_cast<GameLevel*>(_world->GetCurrentLevel())->GetStageIdx() > 3)
    Gunman2Button->SetDebugDraw(true);
#endif // DEBUG

}

void AgentStorage::Update(float dt)
{
  UIPanel::Update(dt);

}

void AgentStorage::BeginLevel()
{
  UIPanel::BeginLevel();
  //static_cast<GameLevel*>(_world->GetCurrentLevel())->
  _map = _world->FindGameObjectByType<Map>();
  bool isNotSimulating = !_map->isAssassinationMode &&
                         !_map->isPlacementModeOn &&
                         !_map->isHoveredCharacterChanged;

  BrawlerButton->AddOnHoveredHandler([=]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }

    if (isNotSimulating)
    {
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
    }
  });
  BrawlerButton->AddOnUnHoveredHandler([=]() {
    _bHoverFlag = false;

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
  BrawlerButton->AddOnClickHandler([=]() {
    SoundManager::PlaySound(SoundList::Button_Click);

    if (!IsBrawlerUsing && !_map->isPlacementModeOn)
    {
      IsBrawlerUsing = true;
      _map->TurnOnPlacementMode(CharacterType::kBrawler, kWest);
      BrawlerDefaultImage->SetOpacity(0.f);
      BrawlerHoveredImage->SetOpacity(0.f);
      BrawlerInactiveImage->SetOpacity(1.f);
      BrawlerButton->Deactivate();
    }
  });
  SlasherButton->AddOnHoveredHandler([=]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }

    if (isNotSimulating)
    {
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
    }
  });
  SlasherButton->AddOnUnHoveredHandler([=]() {
    _bHoverFlag = false;

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
  SlasherButton->AddOnClickHandler([=]() {
    SoundManager::PlaySound(SoundList::Button_Click);

    if (!IsSlasherUsing && !_map->isPlacementModeOn)
    {
      IsSlasherUsing = true;
      _map->TurnOnPlacementMode(CharacterType::kSlasher, kWest);
      SlasherDefaultImage->SetOpacity(0.f);
      SlasherHoveredImage->SetOpacity(0.f);
      SlasherInactiveImage->SetOpacity(1.f);
      SlasherButton->Deactivate();
    }
  });
  Gunman1Button->AddOnHoveredHandler([=]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }

    if (isNotSimulating)
    {
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
    }
  });
  Gunman1Button->AddOnUnHoveredHandler([=]() {
    _bHoverFlag = false;

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
  Gunman1Button->AddOnClickHandler([=]() {
    SoundManager::PlaySound(SoundList::Button_Click);

    if (!IsGunman1Using && !_map->isPlacementModeOn)
    {
      IsGunman1Using = true;
      _map->TurnOnPlacementMode(CharacterType::kGunman, kWest);
      Gunman1DefaultImage->SetOpacity(0.f);
      Gunman1HoveredImage->SetOpacity(0.f);
      Gunman1InactiveImage->SetOpacity(1.f);
      Gunman1Button->Deactivate();
    }
  });
  if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() > 3)
  {
    Gunman2Button->AddOnHoveredHandler([=]() {
      if (!_bHoverFlag)
      {
        SoundManager::PlaySound(SoundList::Button_Hover);
        _bHoverFlag = true;
      }

      if (isNotSimulating)
      {
        if (!IsGunman2Using)
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
      }
    });
    Gunman2Button->AddOnUnHoveredHandler([=]() {
      _bHoverFlag = false;

      if (!IsGunman2Using)
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
    Gunman2Button->AddOnClickHandler([=]() {
      SoundManager::PlaySound(SoundList::Button_Click);

      if (!IsGunman2Using && !_map->isPlacementModeOn)
      {
        IsGunman2Using = true;
        _map->TurnOnPlacementMode(CharacterType::kGunman, kWest);
        Gunman2DefaultImage->SetOpacity(0.f);
        Gunman2HoveredImage->SetOpacity(0.f);
        Gunman2InactiveImage->SetOpacity(1.f);
        Gunman2Button->Deactivate();
      }
    });
  }

}


void AgentStorage::ResetAgent()
{
  switch (_map->deleteCharType)
  {
  case CharacterType::kBrawler:
    if (IsBrawlerUsing)
    {
      BrawlerDefaultImage->SetOpacity(1.f);
      BrawlerHoveredImage->SetOpacity(0.f);
      BrawlerInactiveImage->SetOpacity(0.f);
      IsBrawlerUsing = false;
      BrawlerButton->Activate();
    }
    break;
  case CharacterType::kSlasher:
    if (IsSlasherUsing)
    {
      SlasherDefaultImage->SetOpacity(1.f);
      SlasherHoveredImage->SetOpacity(0.f);
      SlasherInactiveImage->SetOpacity(0.f);
      IsSlasherUsing = false;
      SlasherButton->Activate();
    }
    break;
  case CharacterType::kGunman:
    if (IsGunman1Using)
    {
      Gunman1DefaultImage->SetOpacity(1.f);
      Gunman1HoveredImage->SetOpacity(0.f);
      Gunman1InactiveImage->SetOpacity(0.f);
      IsGunman1Using = false;
      Gunman1Button->Activate();
    }
    else if (IsGunman2Using)
    {
      Gunman2DefaultImage->SetOpacity(1.f);
      Gunman2HoveredImage->SetOpacity(0.f);
      Gunman2InactiveImage->SetOpacity(0.f);
      IsGunman2Using = false;
      Gunman2Button->Activate();
    }
    break;
  }
}

