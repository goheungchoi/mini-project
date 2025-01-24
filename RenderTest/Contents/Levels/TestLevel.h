#pragma once 

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"
#include "Contents/GameObjects/TestGameObject.h"

class TestLevel : public Level
{
  ModelHandle sponzaHandle;
  TestGameObject* testObject;
  
public:

  TestLevel()
      : Level("Test Level") {}
	
  virtual void PrepareLevel() override
  {
  }
  virtual void BeginLevel() override 
  { 
    testObject = world->CreateGameObject<TestGameObject>();
  }

  virtual void DestroyLevel() override {}
  virtual void CleanupLevel() override {}

};

