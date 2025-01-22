#pragma once

#include <directxtk/simplemath.h>							// 수학 연산을 단순화하는 클래스들을 제공
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <cmath>
#include <numbers>
#include <algorithm>

namespace MathUtil
{
constexpr XMVECTOR kRight{1.f, 0.f, 0.f, 0.f};
constexpr XMVECTOR kUp{0.f, 1.f, 0.f, 0.f};
constexpr XMVECTOR kFront{0.f, 0.f, 1.f, 0.f};

// Function to rotate a vector towards a target vector
inline XMVECTOR RotateVectorToward(
    const XMVECTOR& source, // The source vector to rotate
    const XMVECTOR& target, // The target vector to rotate toward
    float maxAngleStep      // Maximum angular step in radians
)
{
  // Normalize the source and target vectors
  XMVECTOR normalizedSource = XMVector3Normalize(source);
  XMVECTOR normalizedTarget = XMVector3Normalize(target);

  // Compute the dot product to determine the cosine of the angle between the
  // vectors
  float dotProduct =
      XMVectorGetX(XMVector3Dot(normalizedSource, normalizedTarget));

  // Clamp the dot product to avoid precision issues (values slightly out of
  // [-1, 1])
  dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

  // Calculate the actual angle between the vectors
  float angleBetween = acosf(dotProduct);

  // If the angle is less than or equal to the step, return the target vector
  if (angleBetween <= maxAngleStep)
  {
    return normalizedTarget;
  }

  // Calculate the rotation axis (perpendicular vector)
  XMVECTOR rotationAxis = XMVector3Cross(normalizedSource, normalizedTarget);

  // Normalize the rotation axis
  rotationAxis = XMVector3Normalize(rotationAxis);

  // Create a quaternion representing the step rotation
  float angleToRotate = maxAngleStep;
  XMVECTOR quaternion = XMQuaternionRotationAxis(rotationAxis, angleToRotate);

  // Rotate the source vector by the quaternion
  XMVECTOR rotatedVector = XMVector3Rotate(normalizedSource, quaternion);

  return rotatedVector;
}

struct AABB
{
  XMVECTOR min;
  XMVECTOR max;
};

constexpr float kQuaterPi_f{0.25 * std::numbers::pi};
constexpr float kHalfPi_f{0.5 * std ::numbers::pi};
constexpr float kPi_f{std::numbers::pi_v<float>};
constexpr float kTwoPi_f{2.0 * std::numbers::pi};

constexpr double kQuaterPi{0.25 * std::numbers::pi};
constexpr double kHalfPi{std::numbers::pi * 0.5};
constexpr double kPi{std::numbers::pi};
constexpr double kTwoPi{2.0 * std::numbers::pi};

constexpr float kInvPi_f{std::numbers::inv_pi_v<float>};
constexpr double kInvPi{std::numbers::inv_pi};

constexpr float kE_f{std::numbers::e_v<float>};
constexpr double kE{std::numbers::e};

constexpr float kSqrt2_f{std::numbers::sqrt2_v<float>};
constexpr double kSqrt2{std::numbers::sqrt2};

}
