#pragma once

#include "Core/Common.h"

#include "Animation.h"

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

  void SetAnimation(Animation* animation) { _animation = animation; }
  Animation* GetAnimation() { return _animation; }

  void AddAnimationStateDependency(const StateName&, AnimationState*);
};
