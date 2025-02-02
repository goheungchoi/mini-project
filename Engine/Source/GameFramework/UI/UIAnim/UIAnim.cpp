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
      _pAnimInfo->Frames[_curFrameIndex]; // ���� �ִϸ��̼��� ������
  _lastFrameIndex = _pAnimInfo->Frames.size() - 1; // ������ Index ��ȣ
  int maxFrameCnt = _lastFrameIndex + 1; // �ִϸ��̼��� frame �� ����

  ///////////////
  // m_FrameTime�� fTimeElapsed�� ��������  Frame.Duration���� ũ��
  // m_FrameIndexCurr�� ������Ű�� m_FrameTime�� 0���� �ʱ�ȭ�Ѵ�.
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

  // �̹��������� ������ ����
  _srcRect = {frame.frameRect.x, frame.frameRect.y,
              frame.frameRect.z, frame.frameRect.w};

  // �׸� ������ 0,0,width,height���� �����ϰ� ���� ��ġ�� Transform���� ����
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
