#include "RetryButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

RetryButton::RetryButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _retryBtnImgs[0] = CreateUI<UIImage>(L"RetryBtnImg_Act");
  _retryBtnImgs[0]->SetSprite("2D\\UI\\UI_Retry_Act.png", {1600, 100});

  _retryBtn = CreateUI<UIButton>(L"RetryBtn");
  _retryBtn->SetSize(_retryBtnImgs[0]->GetSize());
  _retryBtn->SetCenterPos({1600, 100});
#ifdef _DEBUG
  _retryBtn->SetDebugDraw(true);
#endif // _DEBUG

  _retryBtn->AddOnClickHandler([this]() {
	  _map->ResetGame();
  });

}

RetryButton::~RetryButton() {}

void RetryButton::Update(float dt)
{
  __super::Update(dt);
}
