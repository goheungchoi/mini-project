#pragma once
#include "Renderer/D2DRenderer/Common.h"

struct FRAME_INFO
{
  Vector4 frameRect{0, 0, 0, 0};
  Vector2 center{0, 0};
  float duration{0.1f};

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
  std::wstring name{L"DEFAULT"};
  std::vector<FRAME_INFO> Frames;

  ANIMATION_INFO() { Frames.reserve(34); }
};

class Anim2DAsset
{
public:
  Anim2DAsset() { Animations.reserve(10); }
  ~Anim2DAsset();

public:
  const ANIMATION_INFO& GetAnimationInfo(std::wstring animName);
  void SetAnimationInfo(const ANIMATION_INFO& animInfo);

private:
  std::wstring name;
  std::vector<ANIMATION_INFO> Animations;
};
