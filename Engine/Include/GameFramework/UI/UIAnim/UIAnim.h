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

  bool _bLoop;	// �ݺ� ����
  float _frameTime = 0.0f;  // ������ ����ð�
  int _lastFrameIndex = -1; // ������ ������ �ε���
  int _curFrameIndex = -1;  // ���� ������ �ε���
  int _prevFrameIndex = -1; // ���� ������ �ε���
  D2D1_RECT_F _srcRect;    // D2D1Bitmap�� Source ����
  D2D1_RECT_F _dstRect;    // RenderTarget�� Destination ����
};
