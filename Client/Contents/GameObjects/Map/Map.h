#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Grid/GridObject.h"
#include "Contents/GameObjects/Map/Characters/Character.h"
#include "Contents/GameObjects/Map/Obstacle/Obstacle.h"

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

	ModelHandle enemyBrawlerModelHandle;
  ModelHandle enemyGunmanModelHandle;

  ModelHandle civilianModelHandle;
  ModelHandle elizaModelHandle;

	ModelHandle allyBrawlerModelHandle;
  ModelHandle allySlasherModelHandle;
  ModelHandle allyGunmanModelHandle;

  ModelHandle obstacleBox01ModelHandle;
  ModelHandle obstacleBox02ModelHandle;
  ModelHandle obstacleDrumModelHandle;
  ModelHandle obstacleDrumOldModelHandle;
  ModelHandle obstacleLionModelHandle;
  ModelHandle obstacleSofaModelHandle;
  ModelHandle obstacleStoolModelHandle;
  ModelHandle obstacleVBoxModelHandle;

public:

  // Game Objects
  GridObject* grid{nullptr};

  std::vector<Character*> enemies;

  std::vector<Character*> allies;

  std::vector<Character*> civilians;

	// Action trigger
	bool isActionTriggered{false};
  struct CharacterInfo
  {
    Faction faction;
    CharacterType type;
    Direction dir;
    uint32_t w, h;
  };
  std::vector<CharacterInfo> record;


	// Placement mode
	bool isPlacementModeOn{false};
  Character* placeholder{nullptr};


	// Selection mode
  bool isHoveredCharacterChanged{false};
  Character* prevHoveredCharacter{nullptr};
  Character* hoveredCharacter{nullptr};

	// Assassination target
  bool isAssassinationMode{false};
  Character* assassinationTarget{nullptr};

  // 
  std::optional<CharacterInfo> tmp;

  CharacterType deleteCharType = kCivilian;

public:

	Map(World* world);
  ~Map();

	void TurnOnPlacementMode(CharacterType type, Direction dir);
  void TurnOffPlacementMode();

  bool bNeedUpdateAttackRange{true};
	void ShowHoveredCharacterRange();
  bool bRangeHided{true};
  void HideHoveredCharacterRange();

  void TurnOnAssassinationMode();
  void TurnOffAssasinationMode();

  int GetNumEnemies();
  int GetNumAllies();
  int GetNumCivilians();

  int GetNumDeadEnemies();
  int GetNumDeadAllies();
  int GetNumDeadCivilians();

	void TriggerAction();

	void ResetGame();

  void PauseGame();
  void ResumeGame();

  bool IsGameFinished();

	void CreateEnemyAt(CharacterType type, uint32_t w, uint32_t h,
                     Direction dir = kNorth);

  void CreateAllyAt(CharacterType type, uint32_t w, uint32_t h,
                    Direction dir = kNorth);

  void CreateCivillianAt(uint32_t w, uint32_t h, Direction dir = kNorth, bool isEliza = false);

  void CreateObstacleAt(ObstacleType type, uint32_t w, uint32_t h, Direction dir = kNorth);

  void DeleteCharacterFromMap(Character* character);

	void OnAwake() override;

	void Update(float dt) override;
  void PostUpdate(float dt) override;

private:
  XMVECTOR GetCursorPosition() const;
  void TranslatePlaceholder();

  void AssassinateTarget();
};

