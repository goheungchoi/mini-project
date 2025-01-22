#pragma once
//#include "physx/PxPhysicsAPI.h"
//#include "physx/PxPhysics.h"

#include "IPhyjixEngine.h"
using namespace physx;
class PhyjixEngine : public IPhyjixEngine
{
public:
	bool Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;

	IPhyjixWorld* CreateWorld() override;
	void DestroyWorld(IPhyjixWorld* world) override;

private:
	PxFoundation* _pFoundation = nullptr;
	PxPhysics* _pPhysics = nullptr;
	PxDefaultAllocator _defaultAllocatorCallback;
	PxDefaultErrorCallback _defaultErrorCallback;
	PxTolerancesScale _toleranceScale;
	PxDefaultCpuDispatcher* _pDispatcher = nullptr;
	std::vector<IPhyjixWorld*> _worlds;
	UINT _currentWorldIndex = 0;

	//for debug
#ifdef _DEBUG
	PxPvdTransport* _transport = nullptr;
	PxPvd* _pvd = nullptr;
#endif

};

