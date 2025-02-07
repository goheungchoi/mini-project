#include "GameFramework/Components/TrailComponent.h"
#include "GameFramework/World/World.h"
TrailComponent::TrailComponent(GameObject* owner) : ComponentBase(owner)
{
  GetWorld()->_renderer->CreateTrail(trail);
}

TrailComponent::~TrailComponent()
{
	
}

void TrailComponent::Update(float dt)
{
  
}
