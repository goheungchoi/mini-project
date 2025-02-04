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
  
  void SetTexture(TextureHandle handle) { billboard->SetTexture(handle);}
  void SetPosition(XMVECTOR position) { billboard->SetPosition(position);}
  void SetScale(XMVECTOR scale) { billboard->SetSize(scale);}
  bool isVisible{true};
  Billboard* billboard = nullptr;
};
