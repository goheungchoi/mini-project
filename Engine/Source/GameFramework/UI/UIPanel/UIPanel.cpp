#include "GameFramework/UI/UIPanel/UIPanel.h"

UIPanel::UIPanel(class World* world) : UIElement(world)
{
  for (auto ui : uiList)
  {
    ui->Activate();
  }
}

UIPanel::~UIPanel()
{
  for (auto ui : uiList)
  {
    delete ui;
    ui = nullptr;
  }
}

void UIPanel::BeginLevel()
{
  __super::BeginLevel();

  for (auto ui : uiList)
  {
    if (ui->GetStatus() == EStatus_Awake)
      ui->BeginLevel();
  }
}

void UIPanel::Update(float dt)
{
  if (GetStatus() == EStatus_Active )
  {
    for (auto ui : uiList)
    {
      if (ui->GetStatus() == EStatus_Active )
      {
        ui->Update(dt);
      }
    }
  }
}

void UIPanel::Render()
{
  if (GetStatus() == EStatus_Active )
  {
    for (auto ui : uiList)
    {
      if (ui->GetStatus() == EStatus_Active )
      {
        ui->Render();
      }
    }
  }
}
