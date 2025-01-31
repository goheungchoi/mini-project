#pragma once

#include "ComponentBase.h"

#include "Core/Handle/ResourceHandle.h"
#include "Core/Types/Billboard.h"

class BillboardComponent : public ComponentBase
{
public:
  BillboardComponent(class GameObject* owner) : ComponentBase(owner) {}
  virtual ~BillboardComponent()
  {
    if (billboard)
    {
      delete billboard;
    }
  }
  bool isVisible{true};

  Billboard* billboard = nullptr;
};