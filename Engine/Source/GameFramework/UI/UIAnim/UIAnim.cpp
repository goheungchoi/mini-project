#include "GameFramework/UI/UIAnim/UIAnim.h"

UIAnim::UIAnim(World* world) : UIElement(world) {}

UIAnim::~UIAnim() {}

void UIAnim::Update(float dt)
{
  if (_pAnim2DAsset == nullptr || _pAnimInfo == nullptr)
  {
    return;
  }

  const FRAME_INFO& frame =
      _pAnimInfo->Frames[_curFrameIndex]; // 현재 애니메이션의 프레임
  _lastFrameIndex = _pAnimInfo->Frames.size() - 1; // 마지막 Index 번호
  int maxFrameCnt = _lastFrameIndex + 1; // 애니메이션의 frame 총 갯수

  ///////////////
  // m_FrameTime에 fTimeElapsed을 누적시켜  Frame.Duration보다 크면
  // m_FrameIndexCurr를 증가시키고 m_FrameTime을 0으로 초기화한다.
  if (frame.duration == 0 && _curFrameIndex == _lastFrameIndex)
  {
    return;
  }

  _frameTime += dt;

  if (_frameTime >= frame.duration)
  {
    if (_bLoop)
    {
      _curFrameIndex = (_curFrameIndex + 1) % maxFrameCnt;
    }
    else
    {
      _curFrameIndex = std::min(_curFrameIndex + 1, _lastFrameIndex);
    }
    _frameTime -= frame.duration;
  }

  // 이미지에서의 프레임 영역
  _srcRect = {frame.frameRect.x, frame.frameRect.y,
              frame.frameRect.z, frame.frameRect.w};

  // 그릴 영역을 0,0,width,height으로 설정하고 실제 위치는 Transform으로 설정
  _dstRect = {0, 0, _srcRect.right - _srcRect.left,
              _srcRect.bottom - _srcRect.top};
}

void UIAnim::Render()
{
  if (_pAnimInfo == nullptr) return;

  //_renderer->GetRenderTarget()->SetTransform(Transform);
  //_renderer->DrawBitmap(m_pBitmap, m_DstRect, m_SrcRect, 1.0f,
  //                      D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void UIAnim::LoadAnim2DAsset(LPCSTR spritePath, LPCSTR animCSV) {}

void UIAnim::SetAnim2D(std::wstring animName, bool bLoop) {}
