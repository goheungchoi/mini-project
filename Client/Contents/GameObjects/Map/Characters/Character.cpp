#include "Character.h"

#include "GameFramework/World/World.h"
#include "Core/Camera/Camera.h"

#include "Contents/GameObjects/Map/Grid/GridObject.h"

const std::string kFactionTags[3] = {"Ally", "Enemy", "Neutral"};

static Animation* anim;
static AnimationState* state;

int Character::count = 0;

ModelData* Character::enemyModelData{nullptr};
SkeletonHandle Character::enemySkeletonHandle;

ModelData* Character::playerModelData{nullptr};
SkeletonHandle Character::playerSkeletonHandle;

ModelData* Character::civilianModelData{nullptr};
SkeletonHandle Character::civilianSkeletonHandle;
AnimationHandle Character::civilianIdleAnimation;
AnimationHandle Character::civilianSurrenderAnimation;
AnimationHandle Character::civilianDeadAnimation;

AnimationHandle Character::deadAnimation;
AnimationHandle Character::idleAnimation;

AnimationHandle Character::brawlerActionAnimation;

AnimationHandle Character::slashReadyAnimation;
AnimationHandle Character::slashActionAnimation;

AnimationHandle Character::gunReady1Animation;
AnimationHandle Character::gunReady2Animation;
AnimationHandle Character::gunFireAnimation;

Character::Character(World* world) : GameObject(world)
{
  count++;
	
	// Create an Animator component
  animator = CreateComponent<AnimatorComponent>();
  animator->BindSkeleton(playerSkeletonHandle);
  
  animator->DeclareVariable<bool>("dead");
  animator->DeclareVariable<bool>("triggered");
  animator->DeclareVariable<bool>("done");
}

Character::~Character() {
  if (inactiveIndicator)
    delete inactiveIndicator;

  if (activeIndicator)
    delete activeIndicator;

}

void Character::TriggerAction()
{
  isActionTriggered = true;
  if (isTargetInRange)
		animator->SetVariable<bool>("triggered", true, true);
}

void Character::BindDirectionIndicator(GameObject* directionIndicator) {
  this->directionIndicator = directionIndicator;
  directionIndicator->SetTranslation(0.f, 0.f, -.8f);
  AddChildGameObject(directionIndicator);
}

void Character::BindInactiveIndicator(GameObject* inactiveIndicator)
{
  this->inactiveIndicator = inactiveIndicator;
  inactiveIndicator->SetTranslation(0.f, 2.5f, 0.f);
  AddChildGameObject(inactiveIndicator);
}

void Character::BindActiveIndicator(GameObject* activeIndicator)
{
  this->activeIndicator = activeIndicator;
  activeIndicator->SetTranslation(0.f, 2.5f, 0.f);
  AddChildGameObject(activeIndicator);
}

void Character::SetFaction(Faction faction) {
  this->faction = faction;
	SetGameObjectTag(kFactionTags[faction]);

	if (faction == Faction::kAlly)
  {
    animator->BindSkeleton(playerSkeletonHandle);
	}
	else if (faction == Faction::kEnemy)
  {
    animator->BindSkeleton(enemySkeletonHandle);
	}
}

Faction Character::GetFaction()
{
  return faction;
}

void Character::SetDirection(Direction direction) {
  this->dir = direction;
  bDirectionChanged = true;
}

Direction Character::GetDirection()
{
  return dir;
}

void Character::SetGridLocation(uint32_t w, uint32_t h)
{
  grid_w = w;
  grid_h = h;
  bGridLocationChanged = true;
}

std::pair<uint32_t, uint32_t> Character::GetGridLocation()
{
  return {grid_w, grid_h};
}

// TODO:
// void Character::OnHit(GameObject* object) {
//	if (object == "bullet")
//	{
//    health -= 1;
//	}
//
//	if (health <= 0)
//  {
//    animator->SetVariable<bool>("dead", true, true);
//		isDead = true;
//	}
//}

void Character::OnBeginOverlap(GameObject* other) {
  GameObject::OnBeginOverlap(other);

  if (!isDead)
  {
    if (other->GetGameObjectTag() == "weapon")
    {
      health -= 1;
	  }

	  if (health <= 0)
    {
      animator->SetVariable<bool>("dead", true, true);
      isDead = true;
	  }
  }
}

void Character::OnAwake()
{
  grid = world->FindGameObjectByType<GridObject>();
  if (!grid)
  {
    throw std::runtime_error("Can'f find grid!");
  }

  if (bGridLocationChanged)
  {
    ApplyChangedGridLocation();
  }

  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }

	auto* bodyRigidBody = CreateComponent<RigidbodyComponent>();
  bodyRigidBody->Initialize({0, 1.0f, 0},
                            DirectX::SimpleMath::Quaternion::Identity,
                            {0.2f, 1.f, 0.2f}, ColliderShape::eCubeCollider,
                            false, true, world->_phyjixWorld);
  bodyRigidBody->EnableDebugDraw();
}

void Character::Update(float dt) {
	if (isDead)
  {
		// TODO: 
		return;
	}

  if (isActionTriggered)
  {
    if (inactiveIndicator && activeIndicator)
    {
      inactiveIndicator->GetComponent<BillboardComponent>()->isVisible = false;
      activeIndicator->GetComponent<BillboardComponent>()->isVisible = false;
    }
    return;
  }
	
	if (bGridLocationChanged)
  {
    ApplyChangedGridLocation();
	}

	if (bDirectionChanged)
  {
    ApplyChangedDirection();
	}

	// Search the direction in range.
	FindTargetInRange();
  if (isTargetInRange)
  {
		// TODO: Indicator on
    if (inactiveIndicator && activeIndicator)
    {
      inactiveIndicator->GetComponent<BillboardComponent>()->isVisible = false;
      activeIndicator->GetComponent<BillboardComponent>()->isVisible = true;
    }
  }
  else
  {
    if (inactiveIndicator && activeIndicator)
    {
      inactiveIndicator->GetComponent<BillboardComponent>()->isVisible = true;
      activeIndicator->GetComponent<BillboardComponent>()->isVisible = false;
    }
	}
}

void Character::PostUpdate(float dt) {

  if (isActionTriggered)
  {
    return;
  }

  if (inactiveIndicator && activeIndicator)
  {
    if (isTargetInRange)
    {
      auto* billboard = activeIndicator->GetComponent<BillboardComponent>();
      billboard->SetPosition(
          activeIndicator->transform->GetGlobalTranslation());
    }
    else
    {
      auto* billboard = inactiveIndicator->GetComponent<BillboardComponent>();
      billboard->SetPosition(
          inactiveIndicator->transform->GetGlobalTranslation());
    }
  }
}

void Character::ApplyChangedGridLocation() {
  count;
	// Place the game object on the grid.
  grid->MoveGameObjectTo(this, grid_w, grid_h);
	// Translate the game object.
  auto [x, z] = grid->GetActualPositionAt(grid_w, grid_h);
  SetTranslation(x, 0, z);
	// Turn off the flag.
  bGridLocationChanged = false;
}

void Character::ApplyChangedDirection() {
  switch (dir)
  {
  case kNorth:
    SetRotationAroundYAxis(XM_PI);
    break;
  case kEast:
    SetRotationAroundYAxis(XM_PI + XM_PIDIV2);
    break;
  case kSouth:
    SetRotationAroundYAxis(0);
    break;
  case kWest:
    SetRotationAroundYAxis(XM_PIDIV2);
    break;
  }

	bDirectionChanged = false;
}

void Character::FindTargetInRange() {
  int search_dir_w{0}, search_dir_h{0};
	switch (dir)
  {
  case kNorth:
    search_dir_h = 1;
    break;
  case kEast:
    search_dir_w = 1;
    break;
  case kSouth:
    search_dir_h = -1;
    break;
  case kWest:
    search_dir_w = -1;
    break;
  }

	int next_search_spot_w = grid_w;
  int next_search_spot_h = grid_h;
	for (int i = 1; i <= range; ++i)
  {
    next_search_spot_w += search_dir_w;
    next_search_spot_h += search_dir_h;

		if (0 <= next_search_spot_w && next_search_spot_w < grid->GetWidth() &&
        0 <= next_search_spot_h && next_search_spot_h < grid->GetHeight())
    {
      GameObject* searchTarget =
          grid->GetGameObjectAt(next_search_spot_w, next_search_spot_h);

			// No target found at the location.
			if (!searchTarget)
        continue;

			// Don't attack civilians.
			if (searchTarget->GetGameObjectTag() == kFactionTags[kNeutral])
        continue;

			// If this is an opponent.
			if (kFactionTags[!this->faction] == searchTarget->GetGameObjectTag())
      {
        distanceToTarget = i;
        isTargetInRange = true;
        return;
      }
      // If this is an ally.
      else
      {
        // Don't attack.
        distanceToTarget = -1;
        isTargetInRange = false;
        return;
      }
		}
	}

	distanceToTarget = -1;
	isTargetInRange = false;
}
