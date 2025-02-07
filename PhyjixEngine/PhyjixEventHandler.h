#pragma once

class PhyjixEventHandler : public physx::PxSimulationEventCallback
{
public:
  // 충돌 이벤트 처리
  void onContact(const physx::PxContactPairHeader& pairHeader,
                 const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
  // Trigger 이벤트 처리
  void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
  void onWake(physx::PxActor**, physx::PxU32) override;
  void onSleep(physx::PxActor**, physx::PxU32) override;

  // 기타 이벤트 (사용하지 않을 경우 빈 구현)
  void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
  void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}
};
