#include "InGameUI.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"

#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/InGameUI/PlayButton/PlayButton.h"
#include "Contents/UI/InGameUI/RetryButton/RetryButton.h"
#include "Contents/UI/InGameUI/ApplyButton/ApplyButton.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "GameFramework/UI/UICursor/UICursor.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  _mainMission = CreateUI<MainMission>(L"MainMission");
  _subMission = CreateUI<SubMission>(L"SubMission");
  
  _playBtn = CreateUI<PlayButton>(L"PlayBtn");
  _retryBtn = CreateUI<RetryButton>(L"RetryBtn");
  _applyBtn = CreateUI<ApplyButton>(L"ApplyBtn");

  _gunfireBtn = CreateUI<GunfireButton>(L"GunfireBtn");

  _agentStorage = CreateUI<AgentStorage>(L"AgentStorage");

  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);

}

InGameUI::~InGameUI() {}

void InGameUI::Update(float dt)
{
  __super::Update(dt);

  if (_map)
  {
    if (!_map->IsGameFinished())
    {
      ShowUI(L"PlayBtn");
      HideUI(L"RetryBtn");
      HideUI(L"ApplyBtn");
    }
    else
    {
      HideUI(L"PlayBtn");
      ShowUI(L"RetryBtn");
      ShowUI(L"ApplyBtn");
    }
  }



}
