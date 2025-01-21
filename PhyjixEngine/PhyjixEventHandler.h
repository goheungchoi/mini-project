#pragma once
//#include "physx/PxPhysics.h"
class PhyjixEventHandler : public PxSimulationEventCallback
{
public:
  // 충돌 이벤트 처리
  void onContact(const PxContactPairHeader& pairHeader,
                 const PxContactPair* pairs, PxU32 nbPairs) override;
  // Trigger 이벤트 처리
  void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
  void onWake(PxActor**, PxU32) override;
  void onSleep(PxActor**, PxU32) override;

  // 기타 이벤트 (사용하지 않을 경우 빈 구현)
  void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
  void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}
};
