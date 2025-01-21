#pragma once

#include "Core/Math/MathUtils.h"

/**
 * @brief Camera class
 */
class Camera
{

  static constexpr XMVECTOR UP{0.f, 1.f, 0.f, 0.f};
  static constexpr XMVECTOR RIGHT{1.f, 0.f, 0.f, 0.f};
  static constexpr XMVECTOR FORWARD{0.f, 0.f, 1.f, 0.f};

  float vfov;
  UINT width, height;
  float aspectRatio;
  float nearPlane;
  float farPlane;
  XMMATRIX proj;

  float moveSpeed{10.f};
  float rotationSpeed{.5f};

  float downUpMove{0.f};
  float leftRightMove{0.f};
  float backForwardMove{0.f};

  XMVECTOR position{0.f, 0.f, 0.f, 0.f};
  XMVECTOR up{UP};
  XMVECTOR right{RIGHT};
  XMVECTOR forward{FORWARD};
  float pitch{0.f}; // around x-axis in radian
  float yaw{0.f};   // around y-axis in radian
  float roll{0.f};  // around z-axis in radian. Must be 0.f
public:
  Camera(UINT width, UINT height, float vfov = MathUtil::kHalfPi_f,
         float nearPlane = 0.01f, float farPlane = 10000.f)
      : vfov{vfov}, width{width}, height{height},
        aspectRatio{(float)width / (float)height}, nearPlane{nearPlane},
        farPlane{farPlane},
        proj{XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane)}
  {
  }

  void SetWidth(UINT width)
  {
    this->width = width;
    aspectRatio = (float)width / (float)height;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetHeight(UINT height)
  {
    this->height = height;
    aspectRatio = (float)width / (float)height;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetExtent(UINT width, UINT height)
  {
    this->width = width;
    this->height = height;
    aspectRatio = (float)width / (float)height;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetVerticalFieldOfView(float vfov)
  {
    this->vfov = vfov;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetNearPlane(float nearPlane)
  {
    this->nearPlane = nearPlane;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetFarPlane(float farPlane)
  {
    this->farPlane = farPlane;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }
  void SetNearFarPlane(float nearPlane, float farPlane)
  {
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    proj = XMMatrixPerspectiveFovLH(vfov, aspectRatio, nearPlane, farPlane);
  }

  UINT GetWidth() const { return width; }
  UINT GetHeight() const { return height; }
  float GetAspectRatio() const { return aspectRatio; }
  float GetVerticalFieldOfView() const { return vfov; }
  float GetNearPlane() const { return nearPlane; }
  float GetFarPlane() const { return farPlane; }
  const XMMATRIX& GetProjectionMatrix() const { return proj; }

  XMVECTOR GetPosition() const { return position; }
  void SetPosition(XMVECTOR position) { this->position = position; }

  void SetMoveSpeed(float speed) { this->moveSpeed = speed; }
  void AddMoveSpeed(float speed) { this->moveSpeed += speed; }
  void MultiplyMoveSpeed(float m) { this->moveSpeed *= m; }
  float GetMoveSpeed() const { return moveSpeed; }
  void SetRotationSpeed(float speed) { this->rotationSpeed = speed; }
  void AddRotationSpeed(float speed) { this->rotationSpeed += speed; }
  void MultiplyRotationSpeed(float m) { this->rotationSpeed *= m; }
  float GetRotationSpeed() const { return rotationSpeed; }

  void MoveDownUp(float move) { downUpMove += move; }
  void MoveLeftRight(float move) { leftRightMove += move; }
  void MoveBackForward(float move) { backForwardMove += move; }

  void RotateAroundXAxis(float degrees)
  {
    pitch += XMConvertToRadians(degrees);
  }
  void RotateAroundYAxis(float degrees) { yaw += XMConvertToRadians(degrees); }

  void LookAt(XMVECTOR point) {
    // Calculate the direction vector from the camera to the target point
    XMVECTOR direction = XMVectorSubtract(point, position);
    direction = XMVector3Normalize(direction);

    // Extract the components of the direction vector
    float dirX = XMVectorGetX(direction);
    float dirY = XMVectorGetY(direction);
    float dirZ = XMVectorGetZ(direction);

    // Calculate yaw (rotation around the Y-axis)
    yaw = atan2f(dirX, dirZ);

    // Calculate pitch (rotation around the X-axis)
    pitch = asinf(-dirY);

    // Roll remains zero in most LookAt implementations
    roll = 0.0f;
  }

  XMMATRIX GetViewTransform()
  {
    // Rotation
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch * rotationSpeed,
                                                     yaw * rotationSpeed, roll);
    right = XMVector3TransformCoord(RIGHT, rotation);
    forward = XMVector3TransformCoord(FORWARD, rotation);

    // Translation
    position += moveSpeed * downUpMove * UP;
    downUpMove = 0.f;
    position += moveSpeed * leftRightMove * right;
    leftRightMove = 0.f;
    position += moveSpeed * backForwardMove * forward;
    backForwardMove = 0.f;

    return XMMatrixLookToLH(position, forward, UP);
  }
};
