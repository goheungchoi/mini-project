#pragma once

#include "ComponentBase.h"

#include "Core/Math/MathUtils.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"

class TransformComponent : public ComponentBase
{
public:
  // Transform hierarchy
  TransformComponent* parent{nullptr};
  std::list<TransformComponent*> children;

  XMVECTOR scaling{1.f, 1.f, 1.f};
  XMVECTOR quaterion{XMQuaternionIdentity()}; // Rotation in quaternion
  XMVECTOR translation{0.f, 0.f, 0.f};

	bool bNeedUpdateTransform{false};
  XMMATRIX localTransform{XMMatrixIdentity()};

  bool bNeedUpdateGlobalTransform{false};
  XMMATRIX globalTransform{XMMatrixIdentity()};
  
	TransformComponent(class GameObject* owner) : ComponentBase(owner) {}

  void AddChild(TransformComponent* component)
  {
    component->parent = this;
    children.push_back(component);
  }

  void RemoveChild(TransformComponent* component)
  {
    component->parent = nullptr;
    children.remove(component);
  }

	void DetachFromParent() { parent = nullptr; }

	XMVECTOR GetLocalRight() { 
		return XMVector3Rotate(MathUtil::kRight, quaterion);
	}
  XMVECTOR GetLocalUp() { 
		return XMVector3Rotate(MathUtil::kUp, quaterion);
	}
  XMVECTOR GetLocalFront() { 
		return XMVector3Rotate(MathUtil::kFront, quaterion);
	}

	XMVECTOR GetGlobalRight()
  {
    XMMATRIX uniformScaledGlobalTransform{
        XMVector4Normalize(globalTransform.r[0]),
        XMVector4Normalize(globalTransform.r[1]),
        XMVector4Normalize(globalTransform.r[2]), 
				globalTransform.r[3]};
    XMVECTOR globalRotQuat = XMQuaternionRotationMatrix(globalTransform);
    return XMVector3Rotate(MathUtil::kRight, globalRotQuat);
  }
  XMVECTOR GetGlobalUp()
  {
    XMMATRIX uniformScaledGlobalTransform{
        XMVector4Normalize(globalTransform.r[0]),
        XMVector4Normalize(globalTransform.r[1]),
        XMVector4Normalize(globalTransform.r[2]), 
				globalTransform.r[3]};
    XMVECTOR globalRotQuat = XMQuaternionRotationMatrix(globalTransform);
    return XMVector3Rotate(MathUtil::kUp, globalRotQuat);
  }
  XMVECTOR GetGlobalFront()
  {
    XMMATRIX uniformScaledGlobalTransform{
        XMVector4Normalize(globalTransform.r[0]),
        XMVector4Normalize(globalTransform.r[1]),
        XMVector4Normalize(globalTransform.r[2]), 
				globalTransform.r[3]};
    XMVECTOR globalRotQuat = XMQuaternionRotationMatrix(globalTransform);
    return XMVector3Rotate(MathUtil::kFront, globalRotQuat);
  }

	void SetScaling(float scaling) { 
		this->scaling = XMVECTOR{scaling, scaling, scaling};
		bNeedUpdateTransform = true;
	}
	void SetScaling(float x, float y, float z) {
    this->scaling = XMVECTOR{x, y, z};
		bNeedUpdateTransform = true;
	}
  void SetScaling(XMVECTOR scaling)
  {
    this->scaling = scaling;
    bNeedUpdateTransform = true;
	}

	void SetRotation(float pitch, float yaw, float roll) {
    this->quaterion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    bNeedUpdateTransform = true;
	}
  void SetRotation(XMVECTOR euler)
  {
    float pitch = XMConvertToRadians(euler.m128_f32[0]);
    float yaw = XMConvertToRadians(euler.m128_f32[1]);
    float roll = XMConvertToRadians(euler.m128_f32[2]);
    this->quaterion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    bNeedUpdateTransform = true;
	}
  void SetRotationAroundXAxis(float angle) {
    this->quaterion = XMQuaternionRotationAxis(MathUtil::kRight, angle);
    bNeedUpdateTransform = true;
  }
  void SetRotationAroundYAxis(float angle)
  {
    this->quaterion = XMQuaternionRotationAxis(MathUtil::kUp, angle);
    bNeedUpdateTransform = true;
  }
  void SetRotationAroundZAxis(float angle)
  {
    this->quaterion = XMQuaternionRotationAxis(MathUtil::kFront, angle);
    bNeedUpdateTransform = true;
  }
	void SetRotationAroundAxis(XMVECTOR axis, float angle) {
    this->quaterion = XMQuaternionRotationAxis(axis, angle);
    bNeedUpdateTransform = true;
	}

	void SetTranslation(float x, float y, float z) {
    this->translation = XMVECTOR{x, y, z};
    bNeedUpdateTransform = true;
	}
  void SetTranslation(XMVECTOR translation)
  {
    this->translation = translation;
    bNeedUpdateTransform = true;
	}

	void Scale(float scaling)
  {
    this->scaling += XMVECTOR{scaling, scaling, scaling};
    bNeedUpdateTransform = true;
  }
  void Scale(float x, float y, float z)
  {
    this->scaling += XMVECTOR{x, y, z};
    bNeedUpdateTransform = true;
  }
  void Scale(XMVECTOR scaling)
  {
    this->scaling = scaling;
    bNeedUpdateTransform = true;
	}

	void Rotate(float pitch, float yaw, float roll) {
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    bNeedUpdateTransform = true;
	}
  void Rotate(XMVECTOR euler)
  {
    float pitch = XMConvertToRadians(euler.m128_f32[0]);
    float yaw = XMConvertToRadians(euler.m128_f32[1]);
    float roll = XMConvertToRadians(euler.m128_f32[2]);
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    bNeedUpdateTransform = true;
	}
  void RotateAroundXAxis(float angle)
  {
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationAxis(MathUtil::kRight, angle));
    bNeedUpdateTransform = true;
  }
  void RotateAroundYAxis(float angle)
  {
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationAxis(MathUtil::kUp, angle));
    bNeedUpdateTransform = true;
  }
  void RotateAroundZAxis(float angle)
  {
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationAxis(MathUtil::kFront, angle));
    bNeedUpdateTransform = true;
  }
	void RotateAroundAxis(XMVECTOR axis, float angle) {
    this->quaterion = XMQuaternionMultiply(
        this->quaterion, XMQuaternionRotationAxis(axis, angle));
    bNeedUpdateTransform = true;
	}

  /*void RotateToward(XMVECTOR target, float maxAngleStep) {
    this->quaterion = MathUtil::RotateToward(
        this->quaterion, target, GetGlobalTranslation(), maxAngleStep);
    bNeedUpdateTransform = true;
  }*/

	void Translate(float x, float y, float z)
  {
    this->translation += XMVECTOR{x, y, z};
    bNeedUpdateTransform = true;
	}
  void Translate(XMVECTOR translation)
  {
    this->translation += translation;
    bNeedUpdateTransform = true;
	}

  void SetLocalTransform(XMMATRIX transform) { 
    XMVECTOR localTranslation;
    XMVECTOR localQuatRot;
    XMVECTOR localScaling;
    XMMatrixDecompose(&localScaling, &localQuatRot, &localTranslation, transform);

    this->translation = localTranslation;
    this->quaterion = localQuatRot;
    this->scaling = localScaling;

    localTransform = transform; 
    bNeedUpdateTransform = false;
    bNeedUpdateGlobalTransform = true;
  }

	void UpdateLocalTransform()
  {
    XMMATRIX _S = XMMatrixScalingFromVector(scaling);
    XMMATRIX _R = XMMatrixRotationQuaternion(quaterion);
    XMMATRIX _T = XMMatrixTranslationFromVector(translation);

		localTransform = _S * _R * _T;
    bNeedUpdateTransform = false;
    bNeedUpdateGlobalTransform = true;
	}

  void UpdateGlobalTransform()
  {
    if (parent)
      globalTransform = localTransform * parent->globalTransform;
    else
      globalTransform = localTransform;

    bNeedUpdateGlobalTransform = false;
	}

	const XMVECTOR& GetScaling() const { return scaling; }
  const XMVECTOR& GetQuaternion() const { return quaterion; }
  const XMVECTOR& GetTranslation() const { return translation; }
  const XMMATRIX& GetLocalTransform() const { return localTransform; }
  const XMVECTOR& GetGlobalScaling() const {
    return MathUtil::GetScalingFromMatrix(globalTransform);
  }
  const XMVECTOR& GetGlobalQuaternion() const {
    return MathUtil::GetQuaternionFromMatrix(globalTransform);
  }
	const XMVECTOR& GetGlobalTranslation() const {
    return MathUtil::GetTranslationFromMatrix(globalTransform);
	}
  const XMMATRIX& GetGlobalTransform() const { return globalTransform; }
};
