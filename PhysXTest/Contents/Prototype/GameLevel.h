#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "Contents/GameObjects/TestGameObject.h"
#include "GridObject.h"
#include "Character.h"

class GameLevel : public Level
{
	// Resource handles
  ModelHandle redCellModelHandle;
  ModelHandle greenCellModelHandle;
	
	ModelHandle mapMeshHandle;
  
	ModelHandle animTestHandle;


	// GameObjects


  GridObject* grid{nullptr};

	GameObject* map{nullptr};

	Character* character{nullptr};


public:
  GameLevel(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override {
    redCellModelHandle = LoadModel("Models\\RedCell\\RedCell.glb");
    greenCellModelHandle = LoadModel("Models\\GreenCell\\GreenCell.glb");

    animTestHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");

		mapMeshHandle = LoadModel("Models\\Maps\\Map002_Museum.glb");
	}

  virtual void BeginLevel() override {
    grid = world->CreateGameObject<GridObject>();
    grid->CreateGrid(6, 6, 1.4f);

		map = world->CreateGameObjectFromModel(mapMeshHandle);
    map->Translate(0.6f, -0.1f, 0.8f);

		character = world->CreateGameObjectFromModel<Character>(animTestHandle);
    // character->SetRotationAroundXAxis(MathUtil::kQuaterPi_f);
	}

  virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override
  {
    UnloadModel(redCellModelHandle);
    UnloadModel(greenCellModelHandle);

		UnloadModel(mapMeshHandle);
	
		UnloadModel(animTestHandle);
	}
};
