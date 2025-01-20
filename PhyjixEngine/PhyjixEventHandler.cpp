#include "pch.h"
#include "PhyjixEventHandler.h"

void PhyjixEventHandler::SetOnCCD(Event callback)
{
	onCCDCallback = callback;
}

void PhyjixEventHandler::SetOnCollisionEnter(Event callback)
{
	onCollisionEnterCallback = callback;
}

void PhyjixEventHandler::SetOnCollisionExit(Event callback)
{
	onCollisionExitCallback = callback;
}

void PhyjixEventHandler::SetOnTrigger(Event callback)
{
	onTriggerCallback = callback;
}

void PhyjixEventHandler::SetOnAwake(Event callback)
{
	onAwakeCallback = callback;
}

void PhyjixEventHandler::SetOnSleep(Event callback)
{
	onSleepCallback = callback;
}

void PhyjixEventHandler::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			if (onCollisionEnterCallback)
				onCollisionEnterCallback(pairHeader.actors[0], pairHeader.actors[1]);
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			if (onCollisionExitCallback)
				onCollisionExitCallback(pairHeader.actors[0], pairHeader.actors[1]);

	}
}

void PhyjixEventHandler::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void PhyjixEventHandler::onWake(PxActor** pActor, PxU32)
{
}

void PhyjixEventHandler::onSleep(PxActor**, PxU32)
{
}

void PhyjixEventHandler::onCCDContactModify(PxContactModifyPair* const pairs, PxU32 count)
{
}

void PhyjixEventHandler::registerWithScene(PxScene* scene)
{
	scene->setSimulationEventCallback(this);
	scene->setCCDContactModifyCallback(this);
}
