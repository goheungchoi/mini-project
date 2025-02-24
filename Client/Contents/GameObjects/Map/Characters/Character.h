#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Types.h"

enum Faction
{
  kAlly = 0,
	kEnemy = 1,
	kNeutral = 2,
};

extern const std::string kFactionTags[3];

enum CharacterType
{
  kBrawler = 0,
  kSlasher = 1,
  kGunman = 2,
  kCivilian = 3,
	kEliza = 4,
  kBoss = 5,
};

class Character : public GameObject
{
  static int count;

public:

	static ModelData* enemyModelData;
  static SkeletonHandle enemySkeletonHandle;
	
  static ModelData* playerModelData;
	static SkeletonHandle playerSkeletonHandle;

	static ModelData* civilianModelData;
  static SkeletonHandle civilianSkeletonHandle;
  static AnimationHandle civilianIdleAnimation;
  static AnimationHandle civilianSurrenderAnimation;
  static AnimationHandle civilianDeadAnimation;
	
	static AnimationHandle deadAnimation;
  static AnimationHandle idleAnimation;
  
	static AnimationHandle brawlerActionAnimation;
	
	static AnimationHandle slashReadyAnimation;
  static AnimationHandle slashAction1Animation;
  static AnimationHandle slashAction2Animation;
	
	static AnimationHandle gunReady1Animation;
  static AnimationHandle gunReady2Animation;
  static AnimationHandle gunFireAnimation;

  TextureHandle deathIndicatorHandle;
  GameObject* deathIndicator{nullptr};

  bool isDeathIndicatorShown{false};

  TextureHandle tabIndicatorHandle;
  GameObject* tabIndicator{nullptr};

  GameObject* directionIndicator{nullptr};
  GameObject* inactiveIndicator{nullptr};
  GameObject* activeIndicator{nullptr};

  AnimatorComponent* animator;

	// Properties
  bool bGridLocationChanged{false};
  uint32_t grid_w{0}, grid_h{0};

  CharacterType type{};
  int range{0};
  int health{1};

  Faction faction{};

	bool bDirectionChanged{false};
  Direction dir{kEast};

	// TODO: Action only when a target is in range...
  bool isTargetInRange{false};
  int distanceToTarget{-1};

	class GridObject* grid{nullptr};
  class Map* map{nullptr};

	//
	bool isDead{false};

	//
  bool isActionTriggered{false};

  bool isActionFinished{false};


  // isPlacement mode on
  bool isPlacementModeOn{false};

  bool isTransparent{false};

public:

  Character(World* world);

	~Character();

	virtual void TriggerAction();
  bool IsFinishedAction();

  void BindDirectionIndicator(GameObject* directionIndicator);

  void BindInactiveIndicator(GameObject* inactiveIndicator);
  void BindActiveIndicator(GameObject* activeIndicator);

  void ShowDeathIndicator();
  void HideDeathIndicator();

	void SetFaction(Faction faction);
  Faction GetFaction();
  void SetDirection(Direction direction);
  Direction GetDirection();
	void SetGridLocation(uint32_t w, uint32_t h);
  std::pair<uint32_t, uint32_t> GetGridLocation();
  std::pair<int, int> GetGridFrontDirection();

  void SetPlacementMode(bool placementMode);

	// Action
  void Die();

  // Interaction
  bool bHoverDisabled{false};
  void DisableHover();

  void OnHover();
  virtual void OnLeftClick() {};
  virtual void OnRightClick() {};

	void OnBeginOverlap(GameObject* other);

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;

  void PostUpdate(float dt) override;

protected:
	// Update logic
  void ApplyChangedGridLocation();
  void ApplyChangedDirection();

	void FindTargetInRange();
};
