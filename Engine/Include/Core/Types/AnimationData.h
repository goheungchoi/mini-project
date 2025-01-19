#pragma once

#include "Core/Common.h"

#include "Core/Handle/ResourceHandle.h"

#include "Core/Math/MathUtils.h"

struct KeyPosition
{
  XMVECTOR position;
  float timeStamp;
};

struct KeyRotation
{
  /**
   * @brief Orientation angles represented by quaternion.
   * RotationAngle is in radians
   * x = RotationAxis.x * sin(RotationAngle / 2)
   * y = RotationAxis.y * sin(RotationAngle / 2)
   * z = RotationAxis.z * sin(RotationAngle / 2)
   * w = cos(RotationAngle / 2)
   *
   * To get the rotation angle,
   * std::acos(w) * 2.
   */
  XMVECTOR orientation;
  float timeStamp;
};

struct KeyScaling
{
  XMVECTOR scaling;
  float timeStamp;
};

struct AnimationChannel
{
  int boneId; // The id of the bone if it's a skeletal animation's key frame. Otherwise, -1.
  std::string nodeName;	// The name of the node (or bone) influenced by this key frame.

  uint32_t numKeyPositions;
  uint32_t numKeyRotations;
  uint32_t numKeyScalings;
  std::vector<KeyPosition> keyPositions;
  std::vector<KeyRotation> keyRotations;
  std::vector<KeyScaling> keyScalings;
};

struct AnimationData
{
  std::string name;

  float duration;
  float ticksPerSecond;

	XMMATRIX globalInverseTransform;

  std::vector<AnimationChannel> animationChannels;
};

