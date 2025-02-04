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
  physx::PxTolerancesScale tolscale = physx::PxTolerancesScale(1.f);
  _pPhysics =
      PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, tolscale, true, _pvd);

#else
	_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *_pFoundation, physx::PxTolerancesScale());

#endif

	if (!_pPhysics) return false;

	_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

  physx::PxCudaContextManagerDesc cudacmdesc;
  _cudaContextManager = PxCreateCudaContextManager(*_pFoundation, cudacmdesc, PxGetProfilerCallback());
  _particleSystem = _pPhysics->createPBDParticleSystem(*_cudaContextManager, 96);

  //blood effect test
  physx::PxPBDMaterial* pbdmat = _pPhysics->createPBDMaterial(0.2f,0.1f,0.1f,5.0f,0.5f,0.07f,0.05f,0.2f,0.8f,0.3f,1.f);
  const physx::PxU32 particlePhase = _particleSystem->createPhase(pbdmat,physx::PxParticlePhaseFlags(physx::PxParticlePhaseFlag::eParticlePhaseFluid | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));



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
