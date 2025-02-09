#include "ApplyButton.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/InGameUI/InGameUI.h"

ApplyButton::ApplyButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _applyBtnImgs[0] = CreateUI<UIImage>(L"ApplyBtnImg_Act");
  _applyBtnImgs[0]->SetSprite("2D\\UI\\UI_Confirm_Act.png", {1800, 100});

  _applyBtn = CreateUI<UIButton>(L"ApplyBtn");
  _applyBtn->SetSize(_applyBtnImgs[0]->GetSize());
  _applyBtn->SetCenterPos({1800, 100});
#ifdef _DEBUG
  _applyBtn->SetDebugDraw(true);
#endif // _DEBUG

  _applyBtn->AddOnClickHandler([this]() { 
	  //_bApplyflag = true;
    _world->_canvas->HidePanel(L"InGameUI");


	//if (_map->)
	// ReconfirmUI 여기서 활성화 해야함
    _world->_canvas->ShowPanel(L"ResultDialogUI");
	  });
}

ApplyButton::~ApplyButton() {}

void ApplyButton::Update(float dt)
{
  __super::Update(dt);
}
