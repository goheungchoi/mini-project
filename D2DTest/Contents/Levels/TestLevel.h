#pragma once 

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"
#include "Contents/GameObjects/TestGameObject.h"
#define USED2D

class TestLevel : public Level
{
  ModelHandle sponzaHandle;
  GameObject* testObject;
  TestGameObject* text;

public:

  TestLevel()
      : Level("Test Level") {}
	
  virtual void PrepareLevel() override
  {
    //sponzaHandle = LoadModel("Models\\GlbTest\\glbTest.glb");
  }
  virtual void BeginLevel() override {
   // testObject = world->CreateGameObjectFromModel(sponzaHandle);

#ifdef USED2D
    text = world->CreateGameObject<TestGameObject>();
#endif // USED2D
  }

  virtual void DestroyLevel() override {}
  virtual void CleanupLevel() override {}

};

