#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

// ���� ����
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
- ����� ��ȯ�� �� ����, Canvas�� �����ǰ� �ٽ� �����Ǹ鼭
- �� ��鿡 �´� Panel���� �����ȴ�.
*/