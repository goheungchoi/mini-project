#pragma once
//#include "physx/PxPhysics.h"
class PhyjixEventHandler : public PxSimulationEventCallback
{
public:
  // �浹 �̺�Ʈ ó��
  void onContact(const PxContactPairHeader& pairHeader,
                 const PxContactPair* pairs, PxU32 nbPairs) override;
  // Trigger �̺�Ʈ ó��
  void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
  void onWake(PxActor**, PxU32) override;
  void onSleep(PxActor**, PxU32) override;

  // ��Ÿ �̺�Ʈ (������� ���� ��� �� ����)
  void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
  void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}
};
