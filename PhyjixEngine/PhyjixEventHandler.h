#pragma once
class PhyjixEventHandler : public PxSimulationEventCallback , public PxCCDContactModifyCallback
{
	using Event = std::function< void (PxActor* actor1, PxActor* actor2)>;
public:

	void SetOnCCD(Event callback);
	void SetOnCollisionEnter(Event callback);
	void SetOnCollisionExit(Event callback);
	void SetOnTrigger(Event callback);
	void SetOnAwake(Event callback);
	void SetOnSleep(Event callback);




	// 충돌 이벤트 처리
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	// Trigger 이벤트 처리
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

	void onWake(PxActor**, PxU32) override;

	void onSleep(PxActor**, PxU32) override;


	// 기타 이벤트 (사용하지 않을 경우 빈 구현)
	void onConstraintBreak(PxConstraintInfo*, PxU32) override{}


	//ccd 처리
	void onCCDContactModify(PxContactModifyPair* const pairs, PxU32 count) override;

	void registerWithScene(PxScene* scene);
	
private:
	Event onCCDCallback;
	Event onCollisionEnterCallback;
	Event onCollisionExitCallback;
	Event onTriggerCallback;
	Event onAwakeCallback;
	Event onSleepCallback;

};

