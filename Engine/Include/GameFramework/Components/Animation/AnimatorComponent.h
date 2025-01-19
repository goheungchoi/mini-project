#pragma once

#include "GameFramework/Components/ComponentBase.h"

class AnimatorComponent : public ComponentBase
{
//  const AnimationData* _currentAnimation;
//  const SkeletonData* skeleton{nullptr};
//  std::vector<XMMATRIX> _finalBoneTransforms;
//
//public:
//  AnimatorComponent(class GameObject* owner) : ComponentBase{owner} {}
//
//	void BineSkeleton(const std::string& skeletonPath) {
//    auto skeletonHandle = LoadSkeleton(skeletonPath);
//    skeleton = &AccessSkeletonData(skeletonHandle);
//
//		// TODO: _finalBoneTransforms
//	}
//
//	void UnbindSkeleton() { skeleton = nullptr; }
//
//	
//	void UpdateAnimation(float dt)
//  {
//    _deltaTime = dt;
//    if (_currentAnimation)
//    {
//      _currentTime += _currentAnimation->GetTicksPerSecond() * dt;
//      _currentTime = fmod(_currentTime, _currentAnimation->GetDuration());
//      CalculateBoneTransform(&_currentAnimation->GetRootNode(),
//                             XMMatrixIdentity());
//    }
//  }
//
//  void PlayAnimation(Animation* animation)
//  {
//    _currentAnimation = animation;
//    _currentTime = 0.f;
//  }
//
//		/**
//   * @brief Recursively find the world transformations of all bones.
//   * @param node Bone hierarchy node
//   * @param parentTransform
//   */
//  void CalculateBoneTransform(const AnimationNode* node,
//                              XMMATRIX parentTransform)
//  {
//    std::string nodeName = node->name;
//    XMMATRIX nodeTransform = node->transform; // Bone's original local transform
//
//    // Check if this bone is in the current animation;
//    // engaged in this animation by finding it from _bones array of animation
//    Bone* bone = _currentAnimation->FindBone(nodeName);
//    if (bone)
//    {
//      // Update the bone's local transform
//      bone->Update(_currentTime);
//      // Get the node's updated local transform
//      nodeTransform = bone->GetLocalTransform();
//    }
//
//    // Hierarchical transformation
//    XMMATRIX globalTransform = nodeTransform * parentTransform;
//
//    // Store the final bone transform
//    const auto& boneInfoMap = _currentAnimation->GetBoneInfoMap();
//    auto it = boneInfoMap.find(nodeName);
//    if (it != boneInfoMap.end())
//    {
//      const BoneInfo& boneInfo = it->second;
//      int index = boneInfo.id;
//      XMMATRIX offset = boneInfo.offset;
//      // Apply the bone offset from the origin.
//      _finalBoneTransforms[index] = XMMatrixTranspose(offset * globalTransform);
//    }
//
//    // Recursively calculate the child bones' model transform.
//    for (int i = 0; i < node->numChildren; ++i)
//    {
//      CalculateBoneTransform(&node->children[i], globalTransform);
//    }
//  }
//
//  const std::vector<XMMATRIX>& GetFinalBoneTransforms() const
//  {
//    return _finalBoneTransforms;
//  }

};
