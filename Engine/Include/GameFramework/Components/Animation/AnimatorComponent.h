#pragma once

#include "GameFramework/Components/ComponentBase.h"

class AnimatorComponent : public ComponentBase
{
  const AnimationData* _currentAnimation;

  // Skeletal animation
  const SkeletonData* skeleton{nullptr};
  std::vector<XMMATRIX> _finalBoneTransforms;

public:
  AnimatorComponent(class GameObject* owner) : ComponentBase{owner} {}

	void BineSkeleton(SkeletonHandle skeletonHandle) {
    skeleton = &AccessSkeletonData(skeletonHandle);

		// TODO: _finalBoneTransforms
	}

	void UnbindSkeleton() { skeleton = nullptr; }

	
	void UpdateAnimation(float dt)
  {
    /*if (skeleton)
    {
      if (_currentAnimation)
      {
        _currentAnimation += _currentAnimation->GetTicksPerSecond() * dt;
        _currentAnimation =
            fmod(_currentAnimation, _currentAnimation->GetDuration());
        CalculateBoneTransform(&_currentAnimation->GetRootNode(),
                               XMMatrixIdentity());
      }
    }*/

    
  }

  /*void PlayAnimation(Animation* animation)
  {
    _currentAnimation = animation;
    _currentTime = 0.f;
  }*/

};
