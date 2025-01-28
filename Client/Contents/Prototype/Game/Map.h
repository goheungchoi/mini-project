#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/Prototype/Grid/GridObject.h"
#include "Contents/Prototype/Character/Character.h"

class Map : public GameObject
{
	// Resource handle
  ModelHandle animTestHandle;

public:

  // Game Objects
  GridObject* grid{nullptr};

  std::vector<Character*> enemies;

  std::vector<Character*> allies;

  std::vector<Character*> civilians;

	bool isActionTriggered{false};


	// 
	bool isPlacementModeOn{false};
  Character* placeholder;

public:

	Map(World* world);
  ~Map();

	void TurnOnPlacementMode(CharactorType type);
  void TurnOffPlacementMode();

	void TurnOnSimulationMode();

	void TriggerAction();

	void ResetGame();

	void CreateEnemyAt(uint32_t w, uint32_t h, Direction dir = kNorth);

  void CreateAllyAt(CharactorType type, uint32_t w, uint32_t h,
                    Direction dir = kNorth);

  void CreateCivillianAt(uint32_t w, uint32_t h, Direction dir = kNorth);

  void CreateObstacleAt(uint32_t w, uint32_t h);

	void OnAwake() override;

	void Update(float dt) override;
};

