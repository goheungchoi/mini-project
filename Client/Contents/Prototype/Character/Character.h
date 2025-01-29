#pragma once

#include "GameFramework/GameObject/GameObject.h"

enum Faction
{
  kAlly = 0,
	kEnemy = 1,
	kNeutral = 2,
};

extern const std::string kFactionTags[3];

enum CharactorType
{
  kBrawler = 0,
  kSlasher = 1,
  kGunman = 2,
  kCivilian = 3,
};

enum Direction
{
  kNorth = 0, // +z
  kEast = 1,  // +x
  kSouth = 2, // -z
  kWest = 3,	// -x
	kNumDirections
};

class Character : public GameObject
{
  static int count;

public:

  ModelHandle indicatorModelHandle;
  GameObject* indicator;

  ModelHandle characterModelHandle;
  ModelData* characterModelData;
  GameObject* model;
	SkeletonHandle skeletonHandle;
  AnimatorComponent* animator;

	// Properties
  bool bGridLocationChanged{false};
  uint32_t grid_w{0}, grid_h{0};

  CharactorType type{};
  int range{0};
  int health{1};

  Faction faction{};

	bool bDirectionChanged{false};
  Direction dir{kEast};
  bool isTargetInRange{false};

	class GridObject* grid{nullptr};

	//
	bool isDead{false};

public:

  Character(World* world);

	~Character();

	void TriggerAction();

	void SetFaction(Faction faction);
  Faction GetFaction();
  void SetDirection(Direction direction);
  Direction GetDirection();
	void SetGridLocation(uint32_t w, uint32_t h);
  std::pair<uint32_t, uint32_t> GetGridLocation();

  // Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  void OnAwake();
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  void Update(float dt) override;
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}

protected:
	// Update logic
  void ApplyChangedGridLocation();
  void ApplyChangedDirection();

	void FindTargetInRange();
};
