#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Grid/GridObject.h"
#include "Contents/GameObjects/Map/Characters/Character.h"

class Map : public GameObject
{
public:
	// Resource handle
  ModelHandle animTestHandle;

  ModelHandle allyDirectionIndicatorModelHandle;
  ModelHandle enemyDirectionIndicatorModelHandle;

  TextureHandle brawlerInactiveIndicatorTextureHandle;
  TextureHandle brawlerActiveIndicatorTextureHandle;
  TextureHandle slasherInactiveIndicatorTextureHandle;
  TextureHandle slasherActiveIndicatorTextureHandle;
  TextureHandle gunmanInactiveIndicatorTextureHandle;
  TextureHandle gunmanActiveIndicatorTextureHandle;

	ModelHandle enemyGunmanModelHandle;

  ModelHandle playerModelHandle;
  ModelHandle civilianModelHandle;


	ModelHandle allyBrawlerModelHandle;
  ModelHandle allySlasherModelHandle;
  ModelHandle allyGunmanModelHandle;

	ModelHandle clonedAllyBrawlerModelHandle;
	ModelHandle clonedAllySlasherModelHandle;
	ModelHandle clonedAllyGunmanModelHandle;

public:

  // Game Objects
  GridObject* grid{nullptr};

  std::vector<Character*> enemies;

  std::vector<Character*> allies;

  std::vector<Character*> civilians;

	bool isActionTriggered{false};


	// Placement mode
	bool isPlacementModeOn{false};
  Character* placeholder{nullptr};


	// Selection mode
  bool isAnySelected{false};
  Character* selectedCharacter{nullptr};

public:

	Map(World* world);
  ~Map();

	void TurnOnPlacementMode(CharactorType type, Direction dir);
  void TurnOffPlacementMode();

	void TurnOnSimulationMode();

	void TriggerAction();

	void ResetGame();

	void CreateEnemyAt(CharactorType type, uint32_t w, uint32_t h,
                     Direction dir = kNorth);

  void CreateAllyAt(CharactorType type, uint32_t w, uint32_t h,
                    Direction dir = kNorth);

  void CreateCivillianAt(uint32_t w, uint32_t h, Direction dir = kNorth);

  void CreateObstacleAt(uint32_t w, uint32_t h);

	void OnAwake() override;

	void Update(float dt) override;

private:
  XMVECTOR GetCursorPosition() const;
  void TranslatePlaceholder();
};

