#pragma once 

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"
#include "Contents/GameObjects/TestGameObject.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

class TestLevel : public Level
{
  ModelHandle sponzaHandle;
  ModelHandle testAnimationHandle;
  GameObject* testObject{nullptr};
  GameObject* testAnim{nullptr};

  Animation* anim1{};
  Animation* anim2{};

  AnimationState* animState{};

public:

  TestLevel()
      : Level("Test Level") {}
	
  virtual void PrepareLevel() override
  {
    sponzaHandle = LoadModel("Models\\Maps\\Map002_Museum.glb");
    // sponzaHandle = LoadModel("Models\\Sponza\\Sponza.gltf");
    testAnimationHandle = LoadModel("Models\\SkinningTest\\SkinningTest.gltf");
    //testAnimationHandle = LoadModel("AnimTest\\ANI_Test.fbx");
  }

  virtual void BeginLevel() override {
    testObject = world->CreateGameObjectFromModel(sponzaHandle);

    testAnim = world->CreateGameObjectFromModel(testAnimationHandle);

    const ModelData& testAnimModel = AccessModelData(testAnimationHandle);

    anim1 = new Animation(*testAnimModel.animations.begin(), true);
    // anim2 = new Animation(*std::next(testAnimModel.animations.begin()), true);

    animState = new AnimationState(anim1);

    auto* animator = testAnim->CreateComponent<AnimatorComponent>();
    animator->SetState(animState);
    animator->BindSkeleton(testAnimModel.skeleton);

  }

  virtual void DestroyLevel() override {}
  virtual void CleanupLevel() override {}

};

