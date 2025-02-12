#pragma once
#include "GameFramework/Common.h"
#include "Core/Math/MathUtils.h"
#include "GameFramework/World/World.h"

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


  virtual void SetDefaultPos(Vector2 pos);
  virtual void SetCenterPos(Vector2 pos);
  Vector2 GetPosition() { return _position; }

  void SetSize(Vector2 size) { _size = size; }

  std::wstring GetName() { return uiname; }
  void SetName(std::wstring _name) { uiname = _name; }
  Vector2 GetSize() { return _size; }

  EStatus GetStatus() { return _status; }
  void SetStatus(EStatus status) { _status = status; }

  virtual void SetOnActivatedEvent(std::function<void(void)> event)
  {
    OnActivated = event;
  }

  virtual void SetOnDeactivatedEvent(std::function<void(void)> event)
  {
    OnDeactivated = event;
  }


  // State change
  virtual void Activate()
  {
    if (_status == EStatus_Active)
    {
      OnActivated();
      return;
    }
    else
    {
      _status = EStatus_Active;
      OnActivated();
    }

    bShouldActivate = true;
    bShouldDeactivate = false;
    
  }
  virtual void Deactivate()
  {
    if (_status == EStatus_Inactive)
    {
      return;
    }
    else
    {
      OnDeactivated();
      _status = EStatus_Inactive;
    }

    bShouldDeactivate = true;
    bShouldActivate = false;
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
  std::wstring uiname;
  class World* _world = nullptr;
  Vector2 _position{};
  Vector2 _size{100, 100}; // width, height
  float _opacity = 1.0f;

  bool bShouldActivate;
  bool bShouldDeactivate;
  bool bShouldDestroy;

  EStatus _status{EStatus_Awake};
  bool isActive{false};

  std::function<void(void)> OnActivated = []() {};
  std::function<void(void)> OnDeactivated = []() {};



  class UIPanel* _ownerPanel = nullptr;
};
