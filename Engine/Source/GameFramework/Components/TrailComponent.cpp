#include "GameFramework/Components/TrailComponent.h"
#include "GameFramework/World/World.h"
TrailComponent::TrailComponent(GameObject* owner) : ComponentBase(owner)
{
  GetWorld()->_renderer->CreateTrail(trail);
}

TrailComponent::~TrailComponent()
{
  delete trail;
}

void TrailComponent::Update(float dt, DirectX::XMVECTOR currPos, XMVECTOR eye)
{
  trail->UpdateTrail(dt, currPos,eye);
}

void TrailComponent::SetDuration(float duration)
{
  trail->SetMaxDuration(duration);
}

void TrailComponent::SetWidth(float width) 
{
  trail->SetWidth(width);
}

void TrailComponent::SetDirection(DirectX::XMVECTOR direction)
{
  trail->SetDirection(direction);
}