#pragma once

#include "ComponentBase.h"

#include "Core/Math/MathUtils.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"

class TransformComponent : public ComponentBase
{
public:
  // TODO: Transform hierarchy
  TransformComponent* parent;
  std::vector<TransformComponent*> children;

  XMVECTOR scaling{};
  XMVECTOR quaterion{};	// Rotation in quaternion
  XMVECTOR translation{};

	bool bNeedUpdateTransform{false};
  XMMATRIX localTransform{};
  XMMATRIX globalTransform{};
  
	TransformComponent(class GameObject* owner) : ComponentBase(owner) {}

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
    this->quaterion = XMQuaternionRotationRollPitchYawFromVector(euler);
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
    this->quaterion *= XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    bNeedUpdateTransform = true;
	}
  void Rotate(XMVECTOR rotation)
  {
    this->quaterion *= XMQuaternionRotationRollPitchYawFromVector(rotation);
    bNeedUpdateTransform = true;
	}
	void RotateAroundAxis(XMVECTOR axis, float angle) {
    this->quaterion *= XMQuaternionRotationAxis(axis, angle);
    bNeedUpdateTransform = true;
	}

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

	void UpdateLocalTransform()
  {
    XMMATRIX _S = XMMatrixScalingFromVector(scaling);
    XMMATRIX _R = XMMatrixRotationQuaternion(quaterion);
    XMMATRIX _T = XMMatrixTranslationFromVector(translation);

		localTransform = _S * _R * _T;
    bNeedUpdateTransform = false;
	}
  void UpdateGlobalTransform(XMMATRIX parentTransform)
  {
    globalTransform = localTransform * parentTransform;
	}

	const XMVECTOR& GetScaling() const { return scaling; }
  const XMVECTOR& GetQuaternion() const { return quaterion; }
  const XMVECTOR& GetTranslation() const { return translation; }
  const XMMATRIX& GetLocalTransform() const { return localTransform; }
  const XMMATRIX& GetGlobalTransform() const { return globalTransform; }
};
