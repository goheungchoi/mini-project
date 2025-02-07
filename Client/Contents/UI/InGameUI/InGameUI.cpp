#include "InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"

#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/InGameUI/PlayButton/PlayButton.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "GameFramework/UI/UICursor/UICursor.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{
  _mainMission = CreateUI<MainMission>(L"MainMission");
  _subMission = CreateUI<SubMission>(L"SubMission");
  
  _playBtn = CreateUI<PlayButton>(L"PlayBtn");

  _gunfireBtn = CreateUI<GunfireButton>(L"GunfireBtn");

  _agentStorage = CreateUI<AgentStorage>(L"AgentStorage");




  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);

}

InGameUI::~InGameUI() {}
