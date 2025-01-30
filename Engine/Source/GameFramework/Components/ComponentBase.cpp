#include "GameFrameWork/Components/ComponentBase.h"

#include "GameFramework/GameObject/GameObject.h"

World* ComponentBase::GetWorld() const
{
  return _owner->GetWorld();
}
