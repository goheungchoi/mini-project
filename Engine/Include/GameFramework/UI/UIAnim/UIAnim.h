#pragma once
#include "GameFramework/UI/UIElement/UIElement.h"
#include "Renderer/D2DRenderer/Animation2D/Animation2D.h"

class UIAnim : public UIElement
{
public:
  UIAnim(class World* world);
  virtual ~UIAnim();

public:
  void Update(float dt) override;
  void Render() override;

  void LoadAnim2DAsset(LPCSTR spritePath, LPCSTR animCSV);
  void SetAnim2D(std::wstring animName, bool bLoop);

private:
  Anim2DAsset* _pAnim2DAsset;
  ANIMATION_INFO* _pAnimInfo;
  wstring _animCSV;

  bool _bLoop;	// 반복 여부
  float _frameTime = 0.0f;  // 프레임 진행시간
  int _lastFrameIndex = -1; // 마지막 프레임 인덱스
  int _curFrameIndex = -1;  // 현재 프레임 인덱스
  int _prevFrameIndex = -1; // 이전 프레임 인덱스
  D2D1_RECT_F _srcRect;    // D2D1Bitmap의 Source 영역
  D2D1_RECT_F _dstRect;    // RenderTarget의 Destination 영역
};
