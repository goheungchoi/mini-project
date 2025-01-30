#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/World/World.h"

Canvas::Canvas(class World* world) : _world(world) {}

Canvas::~Canvas() {}

void Canvas::BeginLevel()
{
  // Todo : Create UIPanel()
  for (auto panel : panelList)
  {
    if (panel->GetStatus() == EStatus_Awake)
    {
      panel->BeginLevel();
    }
  }
}

void Canvas::Update(float dt)
{
  for (auto panel : panelList)
  {
    if (panel->GetStatus() == EStatus_Active)
    {
      panel->Update(dt);
    }
  }
}

void Canvas::Render()
{
  for (auto panel : panelList)
  {
    if (panel->GetStatus() == EStatus_Active)
    {
      panel->Render();
    }
  }
}
