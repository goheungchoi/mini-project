#pragma once
#include "pch.h"

namespace PhyjixUtil
{
	inline physx::PxVec3 VecToPxVec(const DirectX::SimpleMath::Vector3& vec)
	{
		return physx::PxVec3(vec.x, vec.y, vec.z);
	}
	inline physx::PxVec4 VecToPxVec(const DirectX::SimpleMath::Vector4& vec)
	{
		return physx::PxVec4(vec.x, vec.y, vec.z, vec.w);
	}
	inline DirectX::SimpleMath::Vector3 PxVecToVec(const physx::PxVec3& pxVec)
	{
		return DirectX::SimpleMath::Vector3(pxVec.x, pxVec.y, pxVec.z);
	}
	inline DirectX::SimpleMath::Vector4 PxVecToVec(const physx::PxVec4& pxVec)
	{
		return DirectX::SimpleMath::Vector4(pxVec.x, pxVec.y, pxVec.z, pxVec.w);
	}
}
