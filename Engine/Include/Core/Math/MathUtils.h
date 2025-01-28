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

struct AABB
{
  XMVECTOR min;
  XMVECTOR max;
};

constexpr float kEpsilon_f{1e-6f};

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

inline XMVECTOR RotateToward(XMVECTOR quatRot, XMVECTOR target,
                             XMVECTOR position, float maxAngleStep)
{
  // Calculate the direction from the current position to the target target
  XMVECTOR direction = XMVectorSubtract(target, position);
  direction = XMVector3Normalize(direction);

  // Extract the current forward vector from the quaternion
  XMVECTOR forward = XMVector3Rotate(XMVectorSet(0.f, 0.f, 1.f, 0.f), quatRot);

  // Compute the angle between the forward vector and the target direction
  float dotProduct = XMVectorGetX(XMVector3Dot(forward, direction));
  dotProduct =
      std::clamp(dotProduct, -1.0f, 1.0f); // Clamp to avoid precision issues
  float angleBetween = acosf(dotProduct);

  // If the angle is less than or equal to the maximum step, directly face the
  // target
  if (angleBetween <= maxAngleStep)
  {
    return XMQuaternionRotationMatrix(
        XMMatrixLookToLH(position, direction, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
  }

  // Calculate the rotation axis (perpendicular vector)
  XMVECTOR rotationAxis = XMVector3Cross(forward, direction);
  if (XMVector3LengthSq(rotationAxis).m128_f32[0] < 1e-6f)
  {
    // If the vectors are parallel or anti-parallel, use an arbitrary
    // perpendicular axis
    rotationAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
  }
  rotationAxis = XMVector3Normalize(rotationAxis);

  // Create a quaternion representing the step rotation
  float angleToRotate = (std::min)(maxAngleStep, angleBetween);
  XMVECTOR stepRotation = XMQuaternionRotationAxis(rotationAxis, angleToRotate);

  // Update the current quaternion by applying the step rotation
  XMVECTOR resQuatRot = XMQuaternionMultiply(stepRotation, quatRot);
  return XMQuaternionNormalize(resQuatRot);
}

inline XMVECTOR GetTranslationFromMatrix(const XMMATRIX& matrix)
{
  // Extract the translation vector from the matrix (fourth column)
  return XMVectorSet(
      XMVectorGetX(matrix.r[3]), XMVectorGetY(matrix.r[3]),
      XMVectorGetZ(matrix.r[3]),
      0.0f // Ensure the w-component is 0 for a translation vector
  );
}

inline XMVECTOR GetQuaternionFromMatrix(const XMMATRIX& matrix)
{
  // Extract the rotation quaternion from the transformation matrix
  return XMQuaternionRotationMatrix(matrix);
}

inline XMVECTOR GetScalingFromMatrix(const XMMATRIX& matrix)
{
  // Extract the scaling vector from the transformation matrix
  return XMVectorSet(XMVectorGetX(XMVector3Length(matrix.r[0])),
                     XMVectorGetY(XMVector3Length(matrix.r[1])),
                     XMVectorGetZ(XMVector3Length(matrix.r[2])),
                     0.0f // Ensure the w-component is 0 for a scaling vector
  );
}

}
