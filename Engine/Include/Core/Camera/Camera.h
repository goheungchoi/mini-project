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

  float rotx = 0;
  float rotz = 0;
  float scaleX = 1.0f;
  float scaleY = 1.0f;

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
  XMVECTOR GetPosition() { return position; }
  void SetPosition(XMVECTOR position) { this->position = position; }

  void SetMoveSpeed(float speed) { this->moveSpeed = speed; }
  void AddMoveSpeed(float speed) { this->moveSpeed += speed; }
  void MultiplyMoveSpeed(float m) { this->moveSpeed *= m; }
  float GetMoveSpeed() { return moveSpeed; }
  void SetRotationSpeed(float speed) { this->rotationSpeed = speed; }
  void AddRotationSpeed(float speed) { this->rotationSpeed += speed; }
  void MultiplyRotationSpeed(float m) { this->rotationSpeed *= m; }
  float GetRotationSpeed() { return rotationSpeed; }

  void MoveDownUp(float move) { downUpMove += move; }
  void MoveLeftRight(float move) { leftRightMove += move; }
  void MoveBackForward(float move) { backForwardMove += move; }

  void RotateAroundXAxis(float degrees)
  {
    pitch += XMConvertToRadians(degrees);
  }
  void RotateAroundYAxis(float degrees) { 
		yaw += XMConvertToRadians(degrees); 
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
