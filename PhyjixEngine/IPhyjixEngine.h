#pragma once

class IPhyjixWorld;
class IPhyjixEngine
{
public:
	virtual ~IPhyjixEngine() = default;

	//Initalizing and Terminating
	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

	//Update
	virtual void Update(float delatTime) = 0;

	//Physx Scene Managing
	virtual IPhyjixWorld* CreateWorld() = 0;
	virtual void DestroyWorld(IPhyjixWorld* world) = 0;

};

