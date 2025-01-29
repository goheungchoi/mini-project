#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

// 컨셉 정의
template <typename T>
concept Derived = std::is_base_of<UIPanel, T>::value;

class Canvas
{
public:
  Canvas(class World* world);
  ~Canvas();

private:
  std::map<std::wstring, UIPanel*> panelMap;
  std::vector<class UIPanel*> panelList;
  class World* _world = nullptr;

public:
  void BeginLevel();
  void Update(float dt);
  void Render();

  template <typename T>
    requires Derived<T>
  T* CreatePanel(std::wstring name)
  {
    T* t = new T(_world);
    UIPanel* panel = static_cast<UIPanel*>(t);

    if (panel == nullptr)
    {
      return nullptr;
    }

    panelMap[name] = panel;
    panelList.push_back(panel);

    return t;
  }

  template <typename T>
  T* GetPanel(std::wstring name)
  {
    return dynamic_cast<T*>(panelMap[name]);
  }

  void ShowPanel(std::wstring name) { panelMap[name]->Activate(); }
  void HidePanel(std::wstring name) { panelMap[name]->Deactivate(); }

  void RemovePanel(std::wstring name)
  {
    UIPanel* panel = panelMap[name];
    remove(panelList.begin(), panelList.end(), panel);
    panelMap.erase(name);
    delete panel;
    panel = nullptr;
  }
};

/*
- 장면이 전환될 때 마다, Canvas가 삭제되고 다시 생성되면서
- 그 장면에 맞는 Panel들이 생성된다.
*/