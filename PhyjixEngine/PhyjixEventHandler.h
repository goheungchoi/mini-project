#pragma once

class PhyjixEventHandler : public physx::PxSimulationEventCallback
{
public:
  // �浹 �̺�Ʈ ó��
  void onContact(const physx::PxContactPairHeader& pairHeader,
                 const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
  // Trigger �̺�Ʈ ó��
  void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
  void onWake(physx::PxActor**, physx::PxU32) override;
  void onSleep(physx::PxActor**, physx::PxU32) override;

  // ��Ÿ �̺�Ʈ (������� ���� ��� �� ����)
  void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
  void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}
};
