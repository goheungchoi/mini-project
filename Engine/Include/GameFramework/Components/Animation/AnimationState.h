#pragma once

#include "Core/Common.h"

class Animation;
class AnimatorComponent;
class AnimationState
{
  friend class AnimatorComponent;
  using StateName = std::string;
  using StateDependency = std::unordered_map<StateName, AnimationState*>;

protected:
  StateDependency _stateDependency;
  Animation* _animation;

public:
  AnimationState(Animation* animation);
  virtual ~AnimationState() {}
  virtual void Exit(AnimatorComponent*);
  virtual void Toggle(AnimatorComponent*);
  virtual void Enter(AnimatorComponent*);

  Animation* GetAnimation() { return _animation; }

  void AddAnimationStateDependency(const StateName&, AnimationState*);
};
