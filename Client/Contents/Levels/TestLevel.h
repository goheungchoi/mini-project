#pragma once 

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"
#include "Contents/GameObjects/TestGameObject.h"

class TestLevel : public Level
{
  ModelHandle sponzaHandle;
  GameObject* testObject;

public:

  TestLevel()
      : Level("Test Level") {}
	
  virtual void PrepareLevel() override
  {
    sponzaHandle = LoadModel("Models\\GlbTest\\glbTest.glb");
  }
  virtual void BeginLevel() override {
    testObject = world->CreateGameObjectFromModel(sponzaHandle);
  }

  virtual void DestroyLevel() override {}
  virtual void CleanupLevel() override {}

};

