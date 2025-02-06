#include "AgentStorage.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIButton/UIButton.h"

AgentStorage::AgentStorage(World* world) : UIPanel(world)
{
  _agentStorageBtnImg = CreateUI<UIImage>(L"AgentStorageBtnImg");
  _agentStorageBtnImg->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png",
                                 {1600, 960});
}

AgentStorage::~AgentStorage() {}
