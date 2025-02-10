#pragma once
#include "GameFramework/UI/UIImage/UIImage.h"

struct FRAME_INFO
{
  Vector4 frameRect{0, 0, 0, 0};
  Vector2 center{0, 0};
  float duration{0.05f};
  FRAME_INFO() {}
  FRAME_INFO(Vector4 _frameRect, Vector2 _center, float _duration)
  {
    frameRect = _frameRect;
    center = _center;
    duration = _duration;
  }

  FRAME_INFO(float _left, float _top, float _right, float _bottom,
             float _centerX, float _centerY, float _duration)
  {
    frameRect.x = _left;
    frameRect.y = _top;
    frameRect.z = _right;
    frameRect.w = _bottom;
    center.x = _centerX;
    center.y = _centerY;
    duration = _duration;
  }
};

struct ANIMATION_INFO
{
  std::string name{"DEFAULT"};
  std::vector<FRAME_INFO> Frames;
  bool bIsLoop = true;
  ANIMATION_INFO() { Frames.reserve(34); }
};


struct AnimSprite
{
  Sprite* _pSprite;
  ANIMATION_INFO* _pAnimInfo;
};

class UIAnim : public UIImage
{
public:
  UIAnim(class World* world);
  virtual ~UIAnim();

public:
  void Update(float dt) override;
  void Render() override;

  void SetCurrentAnimSprite(std::string AnimName);
  void LoadAnimSprite(LPCSTR SpritePath, std::string animCSV);
  class Sprite* LoadSprite(LPCSTR SpritePath);
  ANIMATION_INFO* LoadAnim2DAsset(std::string animCSV);


  void SetScale(Vector2 scale);
  void SetScale(std::string spritename, Vector2 scale);

  virtual void SetCenterPos(Vector2 pos) override;
  void SetCenterPos(std::string spritename,Vector2 pos);


  void SetOpacity(std::string spritename, float opacity);

  void SetMasking(Color _col) override;


private:
  std::map<std::string,AnimSprite*> _sprites{};
  std::vector<class Sprite*> _spriteList;
  //class Sprite* _pCurrSprite;
  ANIMATION_INFO* _pCurrAnimInfo; // 현재 애니메이션정보
  FRAME_INFO _pCurrFrameInfo;
  string _animCSV;

  bool _bLoop;	// 반복 여부
  float _frameTime = 0.0f;  // 프레임 진행시간
  int _lastFrameIndex = -1; // 마지막 프레임 인덱스
  int _curFrameIndex = 0;  // 현재 프레임 인덱스
  int _prevFrameIndex = -1; // 이전 프레임 인덱스
  Vector4 _srcRect;    // D2D1Bitmap의 Source 영역
};
