#pragma once

#include "GameFramework/GameObject/GameObject.h"

class TestGameObject : public GameObject
{
  ModelHandle modelHandle;
  ModelData modeldata;
  GameObject* testObj;

public:
	
	TestGameObject(World* world) : GameObject(world) 
	{ 
		modelHandle = LoadModel("Models\\GlbTest\\glbTest.glb");
    modeldata = AccessModelData(modelHandle);
    std::ranges::for_each(modeldata.meshes, [world](MeshHandle handle) 
			{
      world->_renderer->CreateMesh(handle);
      world->_renderer->AddShadow(handle);
			});
	}

	
	void OnRender() override
	{ 
		 std::ranges::for_each(modeldata.meshes, [this](MeshHandle handle) {
      world->_renderer->BeginDraw(handle,Matrix::Identity*Matrix::CreateScale(50.f));
      world->_renderer->DrawMesh(handle);
    });
    
	}

};


