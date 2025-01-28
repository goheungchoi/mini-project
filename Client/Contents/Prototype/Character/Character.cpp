#include "Character.h"

#include "GameFramework/World/World.h"
#include "Core/Camera/Camera.h"

#include "Contents/Prototype/Grid/GridObject.h"

const std::string kFactionTags[3] = {"Ally", "Enemy", "Neutral"};

static Animation* anim;
static AnimationState* state;

int Character::count = 0;

Character::Character(World* world) : GameObject(world)
{
  count++;
	// Fetch the indicator model data.
  indicatorModelHandle = LoadModel("Models\\Indicator\\Indicator.glb");
	// Create an indicator
	// TODO: Indicator game object.
  indicator = world->CreateGameObjectFromModel(indicatorModelHandle);
  indicator->SetTranslation(0.f, 2.22f, 0.f);
  AddChildGameObject(indicator);

	// Fetch the character model data.
  characterModelHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
  characterModelData = &AccessModelData(characterModelHandle);

	// Grab the skeleton handle
  skeletonHandle = characterModelData->skeleton;

	

	// Create an Animator component
  animator = CreateComponent<AnimatorComponent>();
  animator->BindSkeleton(skeletonHandle);
  
  animator->DeclareVariable<bool>("dead");
  animator->DeclareVariable<bool>("triggered");

	// NOTE: Test animation and state.
  /*anim = new Animation(*characterModelData->animations.begin(), true);
  state = new AnimationState(anim);*/
  // animator->SetState(state);
  // animator->AdjustAnimationPlaySpeed(0.1f);
}

Character::~Character() {
  UnloadModel(characterModelHandle);
  UnloadModel(indicatorModelHandle);
  characterModelData = nullptr;
}

void Character::TriggerAction()
{
  animator->SetVariable<bool>("triggered", true, true);
}

void Character::SetFaction(Faction faction) {
  this->faction = faction;
	SetGameObjectTag(kFactionTags[faction]);
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

void Character::OnAwake() {
  grid = world->FindGameObjectByType<GridObject>();
}

// TODO:
//void Character::OnHit(GameObject* object) {
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

void Character::Update(float dt) {

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

	// Search the direction in range.
	FindTargetInRange();
  if (isTargetInRange)
  {
		// TODO: Indicator on
    indicator->Activate();
	}
  else
  {
    indicator->Deactivate();
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
	for (int i = 1; i < range; ++i)
  {
    next_search_spot_w += search_dir_w;
    next_search_spot_h += search_dir_h;

		if (grid->GetWidth() <= next_search_spot_w && next_search_spot_w < grid->GetWidth() &&
        grid->GetHeight() <= next_search_spot_h &&
        next_search_spot_h < grid->GetHeight())
    {
      GameObject* searchTarget =
          grid->GetGameObjectAt(next_search_spot_w, next_search_spot_h);

			// If this is an opponent.
			if (kFactionTags[!this->faction] ==
          searchTarget->GetGameObjectTag())
      {
        isTargetInRange = true;
        return;
			}
		}
	}

	isTargetInRange = false;
}
