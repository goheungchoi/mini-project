#pragma once

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
inline physx::PxQuat VecToPxQuat(const DirectX::SimpleMath::Vector4& vec)
{
  return physx::PxQuat(vec.x, vec.y, vec.z, vec.w);
}

inline DirectX::SimpleMath::Vector3 PxVecToVec(const physx::PxVec3& pxVec)
{
  return DirectX::SimpleMath::Vector3(pxVec.x, pxVec.y, pxVec.z);
}
inline DirectX::SimpleMath::Vector4 PxVecToVec(const physx::PxVec4& pxVec)
{
  return DirectX::SimpleMath::Vector4(pxVec.x, pxVec.y, pxVec.z, pxVec.w);
}
inline DirectX::SimpleMath::Vector4 PxQuatToVec(const physx::PxQuat& pxVec)
{
  return DirectX::SimpleMath::Vector4(pxVec.x, pxVec.y, pxVec.z, pxVec.w);
}
inline DirectX::SimpleMath::Vector2 ScreenToNDC(float mouseX, float mouseY , float windowWidth, float windowHeight)
{
  float x = (2.f * mouseX) / windowWidth - 1.f;
  float y = 1.f - (2.f * mouseY) / windowHeight;
  return DirectX::SimpleMath::Vector2(x, y);
}

inline DirectX::SimpleMath::Vector4 NDCToWorld(DirectX::SimpleMath::Vector2 coord, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
  DirectX::SimpleMath::Vector4 Clip(coord.x, coord.y, -1.f, 1.f);
  DirectX::SimpleMath::Matrix invView = DirectX::XMMatrixInverse(nullptr, view);
  DirectX::SimpleMath::Matrix invProjection = DirectX::XMMatrixInverse(nullptr, projection);

  DirectX::SimpleMath::Vector4 Projection =
      DirectX::XMVector4Transform(Clip, invProjection);
  Projection.z = 1.f;
  Projection.w = 0.f;
  DirectX::SimpleMath::Vector4 World = 
      DirectX::XMVector4Transform(Projection, invView);

  return World;
}


} // namespace PhyjixUtil
