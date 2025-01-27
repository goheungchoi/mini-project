#pragma once
#include "GameFramework/Common.h"
#include "Core/Math/MathUtils.h"

class UIElement
{
public:
  UIElement(class World* world) : _world(world)
  {
    status = EStatus_Awake;
    bShouldActivate = true;
    bShouldDeactivate = false;
    bShouldDestroy = false;
  }

  virtual ~UIElement() = default;

public:
  // Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  virtual void OnAwake() {}
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}



  // State change
  void Activate()
  {
    if (status == EStatus_Active)
      return;

    bShouldActivate = true;
  }
  void Deactivate()
  {
    if (status == EStatus_Inactive)
      return;

    bShouldDeactivate = true;
  }
  void Destroy()
  {
    if (status == EStatus_Cleanup || status == EStatus_Destroyed)
      return;

    bShouldDestroy = true;
  }

  void BeginDestroy()
  {
    status = EStatus_Cleanup;

    // Reset to initial states
    bShouldActivate = false;
    bShouldDeactivate = false;
    bShouldDestroy = false;
  }

  void FinishDestroy()
  {
    status = EStatus_Destroyed;
  }

protected:
  class World* _world = nullptr;
  Vector2 _position{};
  Vector2 _size{};

  bool bShouldActivate;
  bool bShouldDeactivate;
  bool bShouldDestroy;

  EStatus status{EStatus_Awake};
  bool isActive{false};
};
