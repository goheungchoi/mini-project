#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Grid/GridObject.h"
#include "Contents/GameObjects/Map/Characters/Character.h"
#include "Contents/GameObjects/Map/Obstacle/Obstacle.h"

enum BackgroundAudio
{
  kBar,
  kMuseum,
  kWarehouse,
  kDialogue
};

class Map : public GameObject
{
  uint32_t tabCount = 0;

public:
  // Resource handle

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
  ModelHandle enemyBossModelHandle;

  ModelHandle civilianModelHandle;
  ModelHandle elizaModelHandle;

  ModelHandle allyBrawlerModelHandle;
  ModelHandle allySlasherModelHandle;
  ModelHandle allyGunmanModelHandle;

  ModelHandle characterIndicatorModelHandle;

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

  // Transparent indicator.
  std::pair<int, int> indicatorPosition;
  Character* characterIndicator{nullptr};
  GameObject* tempInactiveIndicator{nullptr};

  // Rotate the map back
  bool bRotateBack{false};
  bool bRotating{false};
  const float rotationTime{1.f};
  float rotationElapsedTime{0.f};
  float lastMapAngle{0.f};

  bool bStartAction{false};

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

  bool pause{false};
  void PauseGame();
  void ResumeGame();

  bool IsGameFinished();

  void CreateEnemyAt(CharacterType type, uint32_t w, uint32_t h,
                     Direction dir = kNorth, bool isBoss = false);

  void CreateAllyAt(CharacterType type, uint32_t w, uint32_t h,
                    Direction dir = kNorth);

  void CreateCivillianAt(uint32_t w, uint32_t h, Direction dir = kNorth,
                         bool isEliza = false);

  void CreateObstacleAt(ObstacleType type, uint32_t w, uint32_t h,
                        Direction dir = kNorth);

  void PlaceCharacterIndicatorAt(uint32_t w, uint32_t h, Direction dir = kNorth);

  void DeleteCharacterFromMap(Character* character);

  void PlayBackgroundAudio(BackgroundAudio audio);
  void StopBackgroundAudio();

  std::function<void(void)> OnDeleteCharacter = []() {};

  void OnAwake() override;

  void Update(float dt) override;
  void PostUpdate(float dt) override;
  void OnRender() override;

private:
  XMVECTOR GetCursorPosition() const;
  void TranslatePlaceholder();

  void DetectPlacementAtIndicator();
  void ShowCharacterIndicator();
  void HideCharacterIndicator();

  void AssassinateTarget();

  float mapPos[3]{-4.5f, 0.8f, -4.1f};
  float mapRot = 0.f;
  //XMVECTOR lightStartPoint = {0.804f,-0.864f,0.615f};
  XMVECTOR lightStartPoint = {1.f,-1.f,1.f};
};

