#include "GameFramework/Components/BillboardComponent.h"

#include "GameFramework/World/World.h"

BillboardComponent::BillboardComponent(GameObject* owner) : ComponentBase(owner)
{
  GetWorld()->_renderer->CreateBillboard(this->billboard);
}

BillboardComponent::~BillboardComponent() {

  if (billboard)
  {
    delete billboard;
  }
}
