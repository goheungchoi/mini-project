#pragma once

#include "ResourceManager/ResourceManager.h"

class AnimationChannel
{
  const AnimationChannelData* data;

public:
  XMMATRIX localTransform{};

  AnimationChannel() = default;

  AnimationChannel(const AnimationChannelData* data)
      : data{data}
  {}

  const XMMATRIX& GetLocalTransform() const { return localTransform; }

  const std::string& GetNodeName() const { return data->nodeName; }

  int GetBoneID() const { return data->boneId; }

  uint32_t GetNumKeyPositions() const { return data->numKeyPositions; }
  uint32_t GetNumKeyRotations() const { return data->numKeyRotations; }
  uint32_t GetNumKeyScalings() const { return data->numKeyScalings; }

  const std::vector<KeyPosition>& GetKeyPositions() const
  {
    return data->keyPositions;
  }
  const std::vector<KeyRotation>& GetKeyRotations() const
  {
    return data->keyRotations;
  }
  const std::vector<KeyScaling>& GetKeyScalings() const
  {
    return data->keyScalings;
  };

  void Update(float animationTime) {
    XMMATRIX translate = InterpolatePosition(animationTime);
    XMMATRIX rotation = InterpolateRotation(animationTime);
    XMMATRIX scaling = InterpolateScaling(animationTime);
    localTransform = scaling * rotation * translate;
  }

	/**
   * @brief Find the current index of the keyframe position to interpolate.
   * based on the current animation time.
   * @param animationTime The current animation time
   * @return The index of the keyframe position.
   */
  int GetPositionIndex(float animationTime) const
  {
    for (int index = 0; index < GetNumKeyPositions() - 1; ++index)
    {
      if (animationTime < GetKeyPositions()[index + 1].timeStamp)
        return index;
    }
    return -1;
  }

  /**
   * @brief Find the current index of the keyframe rotation to interpolate.
   * @param animationTime The current animation time
   * @return The index of the keyframe rotation.
   */
  int GetRotationIndex(float animationTime) const
  {
    for (int index = 0; index < GetNumKeyRotations() - 1; ++index)
    {
      if (animationTime < GetKeyRotations()[index + 1].timeStamp)
        return index;
    }
    return -1;
  }

  /**
   * @brief Find the current index of the keyframe scale to interpolate.
   * @param animationTime The current animation time
   * @return The index of the keyframe scale.
   */
  int GetScaleIndex(float animationTime) const
  {
    for (int index = 0; index < GetNumKeyScalings() - 1; ++index)
    {
      if (animationTime < GetKeyScalings()[index + 1].timeStamp)
        return index;
    }
    return -1;
  }

private:
  /**
   * @brief Get the normalized scale factor for lerp and slerp.
   * @param prevKeyframeTimeStamp The time stamp of the previous keyframe.
   * @param nextKeyframeTimeStamp The time stamp of the next keyframe.
   * @param animationTime The current animation time.
   * @return The scale factor in [0, 1].
   */
  float GetScaleFactor(float prevKeyframeTimeStamp, float nextKeyframeTimeStamp,
                       float animationTime)
  {
    float scaleFactor{0.f};
    float midwayLength = animationTime - prevKeyframeTimeStamp;
    float keyframeLength = nextKeyframeTimeStamp - prevKeyframeTimeStamp;
    scaleFactor = midwayLength / keyframeLength;
    return scaleFactor;
  }

  XMMATRIX InterpolatePosition(float animationTime)
  {
    if (GetNumKeyPositions() == 1)
      return XMMatrixTranslationFromVector(GetKeyPositions()[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor =
        GetScaleFactor(GetKeyPositions()[p0Index].timeStamp,
                       GetKeyPositions()[p1Index].timeStamp, animationTime);

    XMVECTOR interpolatedPosition =
        XMVectorLerp(GetKeyPositions()[p0Index].position,
                     GetKeyPositions()[p1Index].position,
                     scaleFactor);

    return XMMatrixTranslationFromVector(interpolatedPosition);
    // return XMMatrixIdentity();
  }

  XMMATRIX InterpolateRotation(float animationTime)
  {
    if (GetNumKeyRotations() == 1)
    {
      // https://users.ncsa.illinois.edu/kindr/emtc/quaternions/
      // For any unit quaternion, q, we have q^(-1) = q^H,
      // where q^H = [w, -v], meaning that the inverse is the conjugate.
      // Rotation of a 3D vector, v, by a unit quaternion, q, is defined as
      // R_q(v) = q[0, v]q^(-1) where [0, v] is a pure quaternion
      // built from v by adding a zero real part.
      XMVECTOR quatRotation =
          XMQuaternionNormalize(GetKeyRotations()[0].orientation);
      return XMMatrixRotationQuaternion(quatRotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor =
        GetScaleFactor(GetKeyRotations()[p0Index].timeStamp,
                       GetKeyRotations()[p1Index].timeStamp, animationTime);

    XMVECTOR interpolatedQuatRotation = XMQuaternionSlerp(
        XMQuaternionNormalize(GetKeyRotations()[p0Index].orientation),
        XMQuaternionNormalize(GetKeyRotations()[p1Index].orientation),
        scaleFactor);
    return XMMatrixRotationQuaternion(interpolatedQuatRotation);
    // return XMMatrixRotationQuaternion(XMQuaternionIdentity());
  }

  XMMATRIX InterpolateScaling(float animationTime)
  {
    if (GetNumKeyScalings() == 1)
    {
      return XMMatrixScalingFromVector(GetKeyScalings()[0].scaling);
    }

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor = GetScaleFactor(
        GetKeyScalings()[p0Index].timeStamp, GetKeyScalings()[p1Index].timeStamp, animationTime);

    XMVECTOR interpolatedScale = XMVectorLerp(GetKeyScalings()[p0Index].scaling,
                     GetKeyScalings()[p1Index].scaling, scaleFactor);
    return XMMatrixScalingFromVector(interpolatedScale);
  }
};

// TODO:
class Animation
{
  const AnimationData* data{nullptr};

  
  std::unordered_map<BoneId, AnimationChannel> boneChannels;
  std::unordered_map<std::string, AnimationChannel> nodeChannels;
  std::unordered_map<BoneId, uint32_t> boneIndexMap;
  std::unordered_map<std::string, uint32_t> nodeIndexMap;

  float _currentAnimTime{0.f}; // Current animation time

  bool isTriggered{false};
  bool isPlaying{false};

public:
  bool isLoop;

  Animation(AnimationHandle handle, bool isLoop = false) : isLoop{isLoop}
  {
    data = &AccessAnimationData(handle);

    boneChannels.reserve(data->animationChannels.size());
    nodeChannels.reserve(data->animationChannels.size());
    boneIndexMap.reserve(data->animationChannels.size());
    nodeIndexMap.reserve(data->animationChannels.size());

    uint32_t index = 0;
    for (const auto& channel : data->animationChannels)
    {
      boneIndexMap[channel.boneId] = index;
      nodeIndexMap[channel.nodeName] = index;
      boneChannels.emplace(channel.boneId, &channel);
      nodeChannels.emplace(channel.nodeName, &channel);
    }
  }

  // const std::vector<AnimationChannel> GetChannels() const { return channels; }

  const AnimationChannel* FindChannel(const std::string& nodeName) const
  {
    if (auto it = nodeChannels.find(nodeName); it != nodeChannels.end())
    {
      return &(it->second);
    }
    else
    {
      return nullptr;
    }
  }

  /**
   * @brief Get the animation speed
   * @return ticks per second
   */
  float GetTicksPerSecond() const { return data->ticksPerSecond; }
  float GetDuration() const { return data->duration; }


  bool IsLoop() const { return isLoop; }
  void SetLoop(float isLoop) { this->isLoop = isLoop; }

  void Trigger()
  {
    isTriggered = true;
    isPlaying = true;

    _currentAnimTime = 0.f;
  }

  void Pause() { isPlaying = false; }

  bool IsPlaying() const { return isPlaying; }

  void Quit()
  {
    isTriggered = false;
    isPlaying = false;
    _currentAnimTime = 0.f;
  }

  void Update(float dt) 
  {
    _currentAnimTime += GetTicksPerSecond() * dt;
    _currentAnimTime = fmod(_currentAnimTime, GetDuration());

    /*for (auto& channel : channels)
    {
      channel.Update(_currentAnimTime);
    }*/
  }

  void UpdateBoneTransforms(float dt, const SkeletonData* skeleton,
                            std::vector<XMMATRIX>& finalBoneTransforms)
  {
    std::vector<XMMATRIX> globalMatrices(finalBoneTransforms.size());

    _currentAnimTime += GetTicksPerSecond() * dt;
    _currentAnimTime = fmod(_currentAnimTime, GetDuration());
    // _currentAnimTime = 30.f;
		// Update the channels first
    for (auto& [_, channel] : boneChannels)
    {
      channel.Update(_currentAnimTime);
    }

		// The root node is always identity.
    // Update the bone transforms of the skeleton
		for (uint32_t i = 0; i < skeleton->nodes.size(); ++i)
    {
      const SkeletonNode& node = skeleton->nodes[i];
      if (node.boneId < 0)
        continue;

			// Check if this bone is in the current animation.
      XMMATRIX nodeTransform{XMMatrixTranspose(node.transform)};
      if (auto it = boneChannels.find(node.boneId); it != boneChannels.end())
      {
        // uint32_t idx = it->second;
				// Get the bone's local transformation.
        if (it->second.GetBoneID() != node.boneId)
        {
          throw std::exception("");
        }

        nodeTransform = it->second.GetLocalTransform();
      }

			// Hierarchical transformation.
			const SkeletonNode& parentNode = skeleton->nodes[node.parent];
      XMMATRIX globalTransform = nodeTransform;
      if (parentNode.boneId >= 0)
        // globalTransform = nodeTransform * parentTransform
        globalTransform *= globalMatrices[parentNode.boneId];

      globalMatrices[node.boneId] = globalTransform;

			// Update it's final bone transformation.
			finalBoneTransforms[node.boneId] =
          XMMatrixTranspose(skeleton->bones[node.boneId].inverseBindMatrix) * globalTransform;
		}
  }

};
