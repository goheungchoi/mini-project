#include "pch.h"
#include "PhyjixEngine.h"

bool PhyjixEngine::Initialize()
{
	_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, _defaultAllocatorCallback, _defaultErrorCallback);
	if (!_pFoundation) return false;

#ifdef _DEBUG
	_pvd = PxCreatePvd(*_pFoundation);

	_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*_transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
#ifdef _DEBUG
	_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, PxTolerancesScale(),true, _pvd);

#else
	_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, PxTolerancesScale());

#endif

	if (!_pPhysics) return false;

	_pDispatcher = PxDefaultCpuDispatcherCreate(2);



	return true;
}

void PhyjixEngine::Terminate()
{
	if (_pDispatcher) _pDispatcher->release();
	if (_pPhysics) _pPhysics->release();
	if (_pFoundation) _pFoundation->release();
}

void PhyjixEngine::Update(float deltaTime)
{
	_worlds[_currentWorldIndex]->Update(deltaTime);
}

IPhyjixWorld* PhyjixEngine::CreateWorld()
{
	auto* world = new PhyjixWorld(_pPhysics, _pDispatcher);
	_worlds.push_back(world);
	return world;
}

void PhyjixEngine::DestroyWorld(IPhyjixWorld* world)
{
	_worlds.erase(std::remove(_worlds.begin(), _worlds.end(), world), _worlds.end());
	delete world;
}
