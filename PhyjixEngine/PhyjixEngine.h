#pragma once

#include "IPhyjixEngine.h"
class PhyjixEngine : public IPhyjixEngine
{
public:
	bool Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;

	IPhyjixWorld* CreateWorld() override;
	void DestroyWorld(IPhyjixWorld* world) override;

private:
  physx::PxFoundation* _pFoundation = nullptr;
  physx::PxPhysics* _pPhysics = nullptr;
  physx::PxDefaultAllocator _defaultAllocatorCallback;
  physx::PxDefaultErrorCallback _defaultErrorCallback;
  physx::PxTolerancesScale _toleranceScale;
  physx::PxDefaultCpuDispatcher* _pDispatcher = nullptr;
  physx::PxCudaContextManager* _cudaContextManager = nullptr;
  physx::PxPBDParticleSystem* _particleSystem = nullptr;

	std::vector<IPhyjixWorld*> _worlds;
	UINT _currentWorldIndex = 0;

	//for debug
#ifdef _DEBUG
#ifdef USEPVD
  physx::PxPvdTransport* _transport = nullptr;
  physx::PxPvd* _pvd = nullptr;
#endif
#endif

};

