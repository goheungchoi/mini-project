#pragma once

#include "GameFramework/Components/ComponentBase.h"


#include "AnimationState.h"

class AnimatorComponent : public ComponentBase
{
  using VariableName = std::string;
  using VariableValue = std::any;
  using AnimationVariables = std::unordered_map<VariableName, std::any>;
  using StateRegistry = std::vector<AnimationState*>;

  AnimationVariables _variables;
  StateRegistry _animationStates;
  AnimationState* _currState{nullptr};

public:
  // Skeletal animation
  const SkeletonData* skeleton{nullptr};
  std::vector<XMMATRIX> finalBoneTransforms;

  AnimatorComponent(class GameObject* owner) : ComponentBase{owner} {}

	void BineSkeleton(SkeletonHandle skeletonHandle) {
    skeleton = &AccessSkeletonData(skeletonHandle);

		finalBoneTransforms.resize(skeleton->bones.size());
	}

	void UnbindSkeleton() { skeleton = nullptr; }

	
	void UpdateAnimation(float dt);

  /*void PlayAnimation(Animation* animation)
  {
    _currentAnimation = animation;
    _currentTime = 0.f;
  }*/

	//
  void Toggle();
  void SetState(AnimationState* state);

  // Animation Controller Variable Modifiers
  template <typename T>
  bool DeclareVariable(VariableName var, const T& init = T{})
  {
    auto it = _variables.find(var);
    if (it != _variables.end())
    {
      return false;
    }
    _variables.insert({var, std::any(init)});
    return true;
  }

  template <typename T>
  bool SetVariable(VariableName var, const T& val, bool toggle = false)
  {
    // Check if the variable exists first.
    auto it = _variables.find(var);
    if (it == _variables.end())
    { // Variable not found
      // TODO: need to log.
      return false;
    }

    // Change the value of the variable
    try
    { // Type check
      T& v = std::any_cast<T&>(it->second);
      v = val;

      // If toggle is true, the current state reacts to
      // the variable changes.
      if (toggle)
        Toggle();
      return true;
    }
    catch (const std::bad_any_cast& e)
    {
      // TODO: need to log.
      return false;
    }
  }

  template <typename T>
  T& GetVariable(VariableName var)
  {
    // Check if the variable exists.
    auto it = _variables.find(var);
    if (it == _variables.end())
    {
      throw std::invalid_argument("Animator: Variable doesn't exist!");
    }

    // Type check
    T& v = std::any_cast<T&>(it->second); // < reference
    return v;
  }

};
