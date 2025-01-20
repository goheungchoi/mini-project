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




	// �浹 �̺�Ʈ ó��
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	// Trigger �̺�Ʈ ó��
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

	void onWake(PxActor**, PxU32) override;

	void onSleep(PxActor**, PxU32) override;


	// ��Ÿ �̺�Ʈ (������� ���� ��� �� ����)
	void onConstraintBreak(PxConstraintInfo*, PxU32) override{}


	//ccd ó��
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

