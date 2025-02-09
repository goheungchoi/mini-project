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


class CubicBezier
{
  double _x1, _y1;
  double _x2, _y2;

public:
  CubicBezier(double x1, double y1, double x2, double y2)
      : _x1{x1}, _y1{y1}, _x2{x2}, _y2{y2}
  {
    if (!((0 <= x1 && x1 <= 1) && (0 <= x2 && x2 <= 1)))
      assert(-1);
  }

  /**
   * @brief
   * @param t Interpolated time t from 0 to 1
   * @return
   */
  double operator()(double t) const
  {
    double s = 1.0 - t;
    double s_squared = s * s;

    double t_squared = t * t;
    double t_cubed = t_squared * t;

    double res1 = 3.0 * s_squared * t * _y1;
    double res2 = 3.0 * s * t_squared * _y2;

    return res1 + res2 + t_cubed;
  }
};

namespace bezier
{
static const CubicBezier linear{0.0, 0.0, 1.0, 1.0};
static const CubicBezier ease{0.25, 0.1, 0.25, 1.0};
static const CubicBezier ease_in{0.42, 0, 1.0, 1.0};
static const CubicBezier ease_out{0, 0, 0.58, 1.0};
static const CubicBezier ease_in_out{0.42, 0, 0.58, 1.0};
}; // namespace bezier

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
  XMVECTOR forward = XMVector3Rotate(XMVectorSet(0.f, 0.f, -1.f, 0.f), quatRot);

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

inline XMMATRIX RotateToward(XMVECTOR target, XMMATRIX transform)
{
  // Extract the position from the transform matrix
  XMVECTOR position = transform.r[3];

  // Compute the direction to the target
  XMVECTOR directionToTarget = XMVector3Normalize(target - position);

  // Extract the forward vector from the transform matrix (assuming it's in the
  // -Z direction)
  XMVECTOR forward = -transform.r[2];

  // Compute the rotation axis (cross product of forward and direction to
  // target)
  XMVECTOR rotationAxis = XMVector3Cross(forward, directionToTarget);

  // Compute the rotation angle (dot product of forward and direction to target)
  XMVECTOR dotProduct = XMVector3Dot(forward, directionToTarget);
  float angle = acos(XMVectorGetX(dotProduct));

  // Create a rotation matrix around the computed axis and angle
  XMMATRIX rotationMatrix = XMMatrixRotationAxis(rotationAxis, angle);

  // Apply the rotation to the original transform
  return rotationMatrix * transform;
}

inline XMMATRIX LookAtLH(XMVECTOR objectPosition, XMVECTOR targetPosition, XMVECTOR up)
{
  // Calculate the direction to the target
  XMVECTOR direction = XMVector3Normalize(targetPosition - objectPosition);

  // Compute the right and new up vectors
  XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, direction));
  XMVECTOR newUp = XMVector3Normalize(XMVector3Cross(direction, right));

  // Construct the rotation matrix
  XMMATRIX rotationMatrix =
      XMMatrixSet(right.m128_f32[0], right.m128_f32[1], right.m128_f32[2], 0.0f,
                  newUp.m128_f32[0], newUp.m128_f32[1], newUp.m128_f32[2], 0.0f,
                  direction.m128_f32[0], direction.m128_f32[1],
                  direction.m128_f32[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

  return rotationMatrix;
}

inline XMVECTOR AxisBillBoardRotate(XMVECTOR forward, XMVECTOR cameraPos,
                             XMVECTOR worldPos)
{
  forward = XMVector3Normalize(forward);

  XMVECTOR targetDir = XMVectorSubtract(cameraPos, worldPos);
  targetDir = XMVector3Normalize(targetDir);

  XMVECTOR axis = XMVector3Cross(targetDir,forward);

    axis = XMVector3Normalize(axis);
  
  return axis;
}
inline float AngleBillBoardRotate(XMVECTOR forward, XMVECTOR cameraPos,
  XMVECTOR worldPos)
{
  forward = XMVector3Normalize(forward);

  XMVECTOR targetDir = XMVectorSubtract(cameraPos, worldPos);
  targetDir = XMVector3Normalize(targetDir);

  float dotProduct = XMVectorGetX(XMVector3Dot(forward, targetDir));

  dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
  float angle = acosf(dotProduct);
  return angle;
}

inline XMMATRIX BillboardMatrix(XMVECTOR objectPos, XMVECTOR cameraPos,
                         XMVECTOR worldUp = XMVectorSet(0, 1, 0, 0))
{
  XMVECTOR forward = XMVector3Normalize(cameraPos - objectPos);
  XMVECTOR right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
  XMVECTOR up = XMVector3Cross(forward, right);

  return XMMATRIX(right.m128_f32[0], right.m128_f32[1], right.m128_f32[2], 0.0f,
                  up.m128_f32[0], up.m128_f32[1], up.m128_f32[2], 0.0f,
                  forward.m128_f32[0], forward.m128_f32[1], forward.m128_f32[2],
                  0.0f, objectPos.m128_f32[0], objectPos.m128_f32[1],
                  objectPos.m128_f32[2], 1.0f);
}

inline XMMATRIX GetBillboardMatrix(XMVECTOR objectPos, XMVECTOR cameraPos,
                            XMVECTOR up = MathUtil::kUp)
{
  // 카메라와 객체 간의 방향 벡터 계산
  XMVECTOR forward = cameraPos - objectPos;
  forward = XMVector3Normalize(forward);

  // 우측 벡터 계산 (기본적으로 X축 방향)
  
  XMVECTOR right = XMVector3Cross(up, forward);
  right = XMVector3Normalize(right);

  // 새로운 Up 벡터 계산
  XMVECTOR newUp = XMVector3Cross(forward, right);
  newUp = XMVector3Normalize(newUp);

  // 4x4 행렬 생성 (LookAt 방식)
  return XMMatrixSet(XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right),
              0.f, XMVectorGetX(newUp), XMVectorGetY(newUp),
              XMVectorGetZ(newUp), 0.f, XMVectorGetX(forward),
              XMVectorGetY(forward), XMVectorGetZ(forward), 0.f,
              XMVectorGetX(objectPos), XMVectorGetY(objectPos),
              XMVectorGetZ(objectPos), 1.f);
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

} // namespace MathUtil
// 회전 축을 계산하는 함수
inline Vector3 AxisBillBoardRotate(const Matrix& local,
                                   const Vector3& cameraPos,
                                   const Vector3& worldPos)
{
  // 로컬 행렬에서 Forward 벡터 추출
  Vector3 forward = local.Forward();
  forward.Normalize();

  // 카메라 방향 벡터
  Vector3 targetDir = cameraPos - worldPos;
  targetDir.Normalize();

  // 회전 축 계산 (Forward와 카메라 방향 벡터의 외적)
  Vector3 axis = forward.Cross(targetDir);
  if (axis.LengthSquared() == 0.0f)
  {
    axis = Vector3(0.f, 1.f, 0.f); // 회전 축이 없을 경우 기본적으로 Y축 사용
  }
  else
  {
    axis.Normalize();
  }

  return axis;
}
