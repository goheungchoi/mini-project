#include "Character.h"

#include "GameFramework/World/World.h"
#include "Core/Camera/Camera.h"

#include "Contents/GameObjects/Map/Map.h"
#include "Contents/GameObjects/Map/Grid/GridObject.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

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

// TODO: Slash action 2;
AnimationHandle Character::slashReadyAnimation;
AnimationHandle Character::slashAction1Animation;
AnimationHandle Character::slashAction2Animation;

AnimationHandle Character::gunReady1Animation;
AnimationHandle Character::gunReady2Animation;
AnimationHandle Character::gunFireAnimation;

Character::Character(World* world) : GameObject(world)
{
  count++;

  // Death indicator.
  deathIndicatorHandle =
      LoadTexture("Textures\\Indicator_Death.png", TextureType::kAlbedo);

  deathIndicator = world->CreateGameObject();
  deathIndicator->SetTranslation(0.f, 2.5f, 0.f);
  {
    auto* bbComp = deathIndicator->CreateComponent<BillboardComponent>();
    bbComp->SetTexture(deathIndicatorHandle);
    bbComp->SetScale({1.f, 1.f, 1.f});
    AddChildGameObject(deathIndicator);
    deathIndicator->SetInvisible();
  }

  // Tab Indicator
  tabIndicatorHandle =
      LoadTexture("Models\\Indicator\\TabIndicator\\UI_Rotate_01.png",
                  TextureType::kAlbedo);

  tabIndicator = world->CreateGameObject();
  tabIndicator->SetTranslation(0.f, 2.5f, 0.f);
  {
    auto* bbComp = tabIndicator->CreateComponent<BillboardComponent>();
    bbComp->SetTexture(tabIndicatorHandle);
    bbComp->SetScale({-.7f, .7f, 1.f});
    AddChildGameObject(tabIndicator);
    tabIndicator->SetInvisible();
  }

	// Create an Animator component
  animator = CreateComponent<AnimatorComponent>();
  animator->BindSkeleton(playerSkeletonHandle);
  
  animator->DeclareVariable<bool>("dead");
  animator->DeclareVariable<bool>("triggered");
  animator->DeclareVariable<bool>("done");
}

Character::~Character() {
  /*if (directionIndicator)
    directionIndicator->Destroy();

  if (inactiveIndicator)
    inactiveIndicator->Destroy();

  if (activeIndicator)
    activeIndicator->Destroy();

  if (tabIndicator)
    tabIndicator->Destroy();

  if (deathIndicator)
    deathIndicator->Destroy();*/

  UnloadTexture(tabIndicatorHandle);
  UnloadTexture(deathIndicatorHandle);
}

void Character::TriggerAction()
{
  isActionTriggered = true;

  HideOutline();
  HideDeathIndicator();

  if (directionIndicator)
  {
    directionIndicator->SetInvisible();
  }

  if (isTargetInRange)
    animator->SetVariable<bool>("triggered", true, true);
  else
    animator->SetVariable<bool>("done", true);
}

bool Character::IsFinishedAction()
{
  return isActionFinished;
}

void Character::BindDirectionIndicator(GameObject* directionIndicator) {
  this->directionIndicator = directionIndicator;
  directionIndicator->SetTranslation(0.f, 0.f, -.5f);
  directionIndicator->SetScaling(.5f);
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

void Character::ShowDeathIndicator() {
  if (deathIndicator)
  {
    deathIndicator->SetVisible();
    isDeathIndicatorShown = true;
  }
}

void Character::HideDeathIndicator() {
  if (deathIndicator)
  {
    deathIndicator->SetInvisible();
    isDeathIndicatorShown = false;
  }
}

void Character::SetFaction(Faction faction) {
  this->faction = faction;
	SetGameObjectTag(kFactionTags[faction]);

	if (faction == Faction::kAlly)
  {
    SetOutlineColor(Color(0.f, 1.f, 0.f));
    animator->BindSkeleton(playerSkeletonHandle);
	}
	else if (faction == Faction::kEnemy)
  {
    SetOutlineColor(Color(1.f, 0.f, 0.f));
    animator->BindSkeleton(enemySkeletonHandle);
  }
  else
  {
    SetOutlineColor(Color(0.f, 0.f, 1.f));
    animator->BindSkeleton(civilianSkeletonHandle);
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

std::pair<int, int> Character::GetGridFrontDirection()
{
  int w_offset{0}, h_offset{0};

	switch (dir)
  {
  case kNorth:
    h_offset = 1;
    break;
  case kEast:
    w_offset = 1;
    break;
  case kSouth:
    h_offset = -1;
    break;
  case kWest:
    w_offset = -1;
    break;
  }

	return {w_offset, h_offset};
}

void Character::SetPlacementMode(bool isPlacementMode) {
  isPlacementModeOn = isPlacementMode;
}

void Character::Die() {
  animator->SetVariable<bool>("dead", true, true);
  isDead = true;

  auto* rbComp = GetComponent<RigidbodyComponent>();
  if (rbComp)
  {
    rbComp->DisableSimulation();
  }

  // 
  if (inactiveIndicator && activeIndicator)
  {
    if (auto bbComp = inactiveIndicator->GetComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->isVisible = false;
    }

    if (auto bbComp = activeIndicator->GetComponent<BillboardComponent>();
        bbComp)
    {
      bbComp->isVisible = false;
    }
  }

  HideDeathIndicator();

  // Play sound.
  if (faction == Faction::kEnemy)
  {
    SoundManager::PlaySound(GetAny(SoundList::Enemy_Die));
  }
  else if (faction == Faction::kAlly)
  {
    switch (type)
    {
    case kBrawler:
      SoundManager::PlaySound(GetAny(SoundList::Ally_Brawler_Die));
      break;
    case kSlasher:
      SoundManager::PlaySound(GetAny(SoundList::Ally_Slasher_Die));
      break;
    case kGunman:
      SoundManager::PlaySound(GetAny(SoundList::Ally_Gunman_Die));
      break;
    }
  }
  else
  {
    SoundManager::PlaySound(GetAny(SoundList::Civilian_Die));
  }
}

void Character::DisableHover() {
  bHoverDisabled = true;
}

void Character::OnHover()
{
  if (map->pause)
    return;

  if (bHoverDisabled)
    return;

  if (isActionTriggered)
    return;

  if (auto* rbComp = GetComponent<RigidbodyComponent>(); rbComp)
  {
    rbComp->debugColor = Color(0, 1, 1, 1);
  }

  if (map->hoveredCharacter != this)
  {
    if (map->prevHoveredCharacter != this)
    {
      SoundManager::PlaySound(SoundList::Character_Hover);
    }

    map->isHoveredCharacterChanged = true;
    map->bNeedUpdateAttackRange = true;
    map->hoveredCharacter = this;
  }
}

void Character::OnBeginOverlap(GameObject* other) {
  GameObject::OnBeginOverlap(other);

  if (map->pause)
    return;

	// Prevent invalid memory access.
	/*if (((uint64_t)other & 0xf000000000000000) != 0)
    return;*/

  if (isPlacementModeOn)
  {
    return;
  }

  if (!isDead)
  {
    if (other->GetGameObjectTag() == "weapon")
    {
      health -= 1;
	  }

	  if (health <= 0)
    {
      Die();
	  }
  }
}

void Character::OnAwake()
{
  grid = world->FindGameObjectByType<GridObject>();
  if (!grid)
  {
    throw std::runtime_error("Can't find grid!");
  }

	map = world->FindGameObjectByType<Map>();
  if (!map)
  {
    throw std::runtime_error("Can't find map!");
	}

  if (bGridLocationChanged)
  {
    ApplyChangedGridLocation();
  }

  if (bDirectionChanged)
  {
    ApplyChangedDirection();
  }

  if (!isPlacementModeOn)
  {
	  auto* bodyRigidBody = CreateComponent<RigidbodyComponent>();
    bodyRigidBody->Initialize({0, 1.0f, 0}, Quaternion::Identity,
                              {0.2f, 1.f, 0.2f}, ColliderShape::eCubeCollider,
                              false, true, world->_phyjixWorld);
    bodyRigidBody->DisableSimulation();
    bodyRigidBody->EnableDebugDraw();
  }
  else
  {
    if (!isTransparent)
      tabIndicator->SetVisible();
  }
}

void Character::Update(float dt)
{
  if (map->pause)
    return;

  if (auto* rbComp = GetComponent<RigidbodyComponent>(); rbComp)
  {
    rbComp->debugColor = Color(1, 0, 1, 1);
    if (rbComp->IsOverlapping())
    {
      rbComp->debugColor = Color(0, 1, 1, 1);
    }
  }

  if (animator->GetVariable<bool>("done"))
  {
    isActionFinished = true;
  }
  else
  {
    isActionFinished = false;
  }
  
  if (isActionTriggered)
  {
    if (!isPlacementModeOn)
    {
      if (auto* rbComp = GetComponent<RigidbodyComponent>())
      {
        rbComp->EnableSimulation();
      }

      if (inactiveIndicator && activeIndicator)
      {
        if (auto bbComp = inactiveIndicator->GetComponent<BillboardComponent>();
            bbComp)
        {
          bbComp->isVisible = false;
        }

        if (auto bbComp = activeIndicator->GetComponent<BillboardComponent>();
            bbComp)
        {
          bbComp->isVisible = false;
        }
      }

      HideDeathIndicator();
    }
  }

  if (isDead)
  {
    // TODO:
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

  if (isPlacementModeOn)
  {
    if (map)
    {
      float myAngle = 0;
      switch (dir)
      {
      case kNorth:
        myAngle = XM_PI+XM_PIDIV2;
        break;
      case kEast:
        myAngle = 0;
        break;
      case kSouth:
        myAngle = XM_PIDIV2;
        break;
      case kWest:
        myAngle = XM_PI;
        break;
      }

      XMVECTOR myQuatRot = XMQuaternionRotationAxis({0.f, 1.f, 0.f}, myAngle);
      XMVECTOR mapQuatRot =
          MathUtil::GetQuaternionFromMatrix(map->GetWorldTransform());
      SetRotationAroundYAxis(XMConvertToDegrees(
          XMVectorGetY(XMQuaternionMultiply(mapQuatRot, myQuatRot)))); 
      SetRotationAroundYAxis(myAngle-acosf(XMVectorGetY(mapQuatRot)));
      
    }
  }

  if (!isActionTriggered)
  {
    if (!isPlacementModeOn)
    {
      // Search the direction in range.
      FindTargetInRange();
      if (isTargetInRange)
      {
        // TODO: Indicator on
        if (inactiveIndicator && activeIndicator)
        {
          inactiveIndicator->GetComponent<BillboardComponent>()->isVisible =
              false;
          activeIndicator->GetComponent<BillboardComponent>()->isVisible = true;
        }
      }
      else
      {
        if (inactiveIndicator && activeIndicator)
        {
          inactiveIndicator->GetComponent<BillboardComponent>()->isVisible =
              true;
          activeIndicator->GetComponent<BillboardComponent>()->isVisible =
              false;
        }
      }
    }
  }

  if (isDeathIndicatorShown)
  {
    if (inactiveIndicator && activeIndicator)
    {
      inactiveIndicator->GetComponent<BillboardComponent>()->isVisible = false;
      activeIndicator->GetComponent<BillboardComponent>()->isVisible = false;
    }
  }
}

void Character::PostUpdate(float dt) {
  if (map->pause)
    return;

  if (isActionTriggered)
  {
    return;
  }

  // Indicator transforms
  if (activeIndicator && isTargetInRange)
  {
    auto* billboard = activeIndicator->GetComponent<BillboardComponent>();
    billboard->SetPosition(activeIndicator->transform->GetGlobalTranslation());
  }

  if (inactiveIndicator && !isTargetInRange)
  {
    auto* billboard = inactiveIndicator->GetComponent<BillboardComponent>();
    billboard->SetPosition(
        inactiveIndicator->transform->GetGlobalTranslation());
  }

  if (deathIndicator)
  {
    auto* billboard = deathIndicator->GetComponent<BillboardComponent>();
    if (billboard)
    {
      billboard->SetPosition(deathIndicator->transform->GetGlobalTranslation() +
                             XMVECTOR{0.f, 0.f, -0.01f});
    }
  }

  if (tabIndicator)
  {
    auto* billboard = tabIndicator->GetComponent<BillboardComponent>();
    if (billboard)
    {
      billboard->SetPosition(tabIndicator->transform->GetGlobalTranslation());
    }
  }
}

void Character::ApplyChangedGridLocation() {
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
      {
        // Don't attack.
        distanceToTarget = -1;
        isTargetInRange = false;
        return;
      }

      // Gunman -> Skip the obstacle
      // Brawler -> Don't attack and return.
      // Slasher -> Don't attack and return.
      if (searchTarget->GetGameObjectTag() == "Obstacle")
      {
        if (type == kGunman)
        {
          continue;
        }
        else
        {
          // Don't attack.
          distanceToTarget = -1;
          isTargetInRange = false;
          return;
        }
      }

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
