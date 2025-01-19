#pragma once 

#include "GameFramework/Components/ComponentBase.h"

class ColliderComponent : public ComponentBase
{

public:

	ColliderComponent(class GameObject* gameObject) : ComponentBase(gameObject) {}

  // Collision
	// void SetCollisionEnabled(CollisionEnabled::Type type);
	// bool IsCollisionEnabled() const;
	// Collision Callbacks
  virtual void OnComponentBeginOverlap() {}
  virtual void OnComponentOverlap() {}
  virtual void OnComponentEndOverlap() {}

		///**
  // * @brief �ٸ� ������Ʈ�� Ư�� ��ġ���� ������Ʈ�� ��ġ���� Ȯ���մϴ�.
  // * @param component ������ �׽�Ʈ�� ������Ʈ
  // * @param pos �ٸ� ������Ʈ�� �׽�Ʈ�� ��ġ
  // * @param rotation �ٸ� ������Ʈ�� �����̼� ��Ʈ����
  // * @return True �̸� ������ �մϴ�.
  // */
  //bool CheckComponentOverlapComponent(PrimitiveComponent* component,
  //                                    const DXVec2& pos,
  //                                    const DXMat4x4& rotation)
  //{
  //  return CheckComponentOverlapComponentImpl(component, pos, rotation);
  //}

  //bool CheckComponentOverlapComponentWithResult(
  //    PrimitiveComponent* component, const DXVec2& pos,
  //    const DXMat4x4& rotation, std::vector<OverlapResult>& outOverlap)
  //{
  //  return CheckComponentOverlapComponentWithResultImpl(component, pos,
  //                                                      rotation, outOverlap);
  //}

  //virtual bool CheckLineTraceComponent(HitResult& outHit, const DXVec2 start,
  //                                     const DXVec2 end, float width)
  //{
  //  // TODO:
  //  return true;
  //}

  //virtual bool CheckSweepComponent(HitResult& outHit, const DXVec2& start,
  //                                 const DXVec2& end, const DXMat4x4& rotation,
  //                                 const CollisionShape& collisionShape,
  //                                 const ECollisionChannel collisionChannel,
  //                                 const CollisionProperty& collisionProperty);

  //virtual bool CheckOverlapComponent(
  //    OverlapResult& outOverlap, const DXVec2& pos, const DXMat4x4& rotation,
  //    const CollisionShape& collisionShape,
  //    const ECollisionChannel collisionChannel,
  //    const CollisionProperty& collisionProperty);

  ///**
  // * @brief Dispatch a blocking hit event to the owner.
  // * @param owner
  // * @param blockingHit
  // */
  //void DispatchBlockingHit(Actor& owner, HitResult& blockingHit);

  ///**
  // * @brief Checks if another component is currently overlapping with this
  // * component
  // * @param otherComp
  // * @return True when overlapping. False, otherwise.
  // */
  //bool IsOverlappingComponent(PrimitiveComponent* otherComp) const
  //{
  //  auto it = currentlyOverlappingComponents.find(otherComp);
  //  return it != currentlyOverlappingComponents.end();
  //}

  ///**
  // * @brief Begin tracking an overlap interaction with the component specified.
  // * @param otherOverlap
  // * @param bDoNotifies
  // */
  //void BeginComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies);

  ///**
  // * @brief Receive begin component overlap event and do all the converting
  // * stuff.
  // */
  //void ReceiveBeginComponentOverlap(PrimitiveComponent* otherComp,
  //                                  const bool bFromSweep,
  //                                  const HitResult& overlapInfo);

  ///**/
  //void ComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies);

  ///**
  // * @brief End tracking an overlap interaction
  // * @param otherOverlap
  // * @param bDoNotifies
  // * @param bSkipNotifySelf
  // */
  //void EndComponentOverlap(const OverlapInfo& overlap, bool bDoNotifies,
  //                         bool bSkipNotifySelf = false);

  ///**
  // * @brief Receive end component overlap event and do all the converting stuff.
  // */
  //void ReceiveEndComponentOverlap(PrimitiveComponent* otherComp);

  //// TODO: Place it in the UpdateOverlaps()
  //bool PushOverlappingComponent(PrimitiveComponent* otherComponent,
  //                              const OverlapInfo& overlapInfo)
  //{
  //  auto [it, res] =
  //      currentlyOverlappingComponents.insert({otherComponent, overlapInfo});
  //  return res;
  //}

  //bool PopOverlappingComponent(PrimitiveComponent* otherComponent)
  //{
  //  std::size_t n = currentlyOverlappingComponents.erase(otherComponent);
  //  return n;
  //}

  ///**
  // * @brief ������ ������Ʈ�� ������Ʈ �մϴ�.
  // */
  //virtual void UpdateOverlaps(const std::vector<OverlapInfo>* overlaps,
  //                            bool bDoNotifies = true);


};
