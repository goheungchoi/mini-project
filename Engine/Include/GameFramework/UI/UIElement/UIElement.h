#pragma once
#include "GameFramework/Common.h"
#include "Core/Math/MathUtils.h"
#include  "GameFramework/World/World.h"

class UIElement
{
public:
  UIElement(class World* world) : _world(world)
  {
    _status = EStatus_Awake;
    bShouldActivate = true;
    bShouldDeactivate = false;
    bShouldDestroy = false;
  }

  virtual ~UIElement() = default;

public:
  virtual void BeginLevel();
  virtual void Update(float dt);
  virtual void Render();

  void SetownerPanel(class UIPanel* ownerPanel) { _ownerPanel = ownerPanel; }
  
  void SetPosition(Vector2 pos) { _position = pos; }
  Vector2 GetPosition() { return _position; }

  void SetSize(Vector2 size) { _size = size; }
  Vector2 GetSize() { return _size; }

  EStatus GetStatus() { return _status; }

  // State change
  void Activate()
  {
    if (_status == EStatus_Active)
      return;

    bShouldActivate = true;
  }
  void Deactivate()
  {
    if (_status == EStatus_Inactive)
      return;

    bShouldDeactivate = true;
  }
  void Destroy()
  {
    if (_status == EStatus_Cleanup || _status == EStatus_Destroyed)
      return;

    bShouldDestroy = true;
  }

  void BeginDestroy()
  {
    _status = EStatus_Cleanup;

    // Reset to initial states
    bShouldActivate = false;
    bShouldDeactivate = false;
    bShouldDestroy = false;
  }

  void FinishDestroy() { _status = EStatus_Destroyed; }

protected:
  class World* _world = nullptr;
  Vector2 _position{};
  Vector2 _size{100, 100};  // width, height

  bool bShouldActivate;
  bool bShouldDeactivate;
  bool bShouldDestroy;

  EStatus _status{EStatus_Awake};
  bool isActive{false};

  class UIPanel* _ownerPanel = nullptr;
};
