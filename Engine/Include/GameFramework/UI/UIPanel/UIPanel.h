#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIElement/UIElement.h"

// 컨셉 정의
template <typename T>
concept UIElementType = std::is_base_of<UIElement, T>::value;

class UIPanel : public UIElement
{
public:
  std::map<std::wstring, UIElement*> uiMap;
  std::list<class UIElement*> uiList;

protected:
  class Map* _map{};

public:
  UIPanel(class World* world);
  virtual ~UIPanel();

public:
  void BeginLevel() override;
  void Update(float dt) override;
  void Render() override;

  //template <typename T>
  //  requires UIElementType<T>
  //T* CreateUI(std::wstring name)
  //{
  //  T* t = new T(_world);
  //  UIElement* ui = static_cast<UIElement*>(t);

  //  if (ui == nullptr)
  //    return nullptr;

  //  uiMap[name] = ui;
  //  uiList.push_back(ui);
  //  ui->SetownerPanel(this);

  //  return t;
  //}

  template <typename T, typename... Args>
    requires UIElementType<T>
  T* CreateUI(std::wstring name, Args&&... args)
  {
    T* t = new T(_world, std::forward<Args>(args)...); // 생성자 인자 전달
    UIElement* ui = static_cast<UIElement*>(t);

    if (!ui)
      return nullptr;
    ui->SetName(name);
    uiMap[name] = ui;
    uiList.push_back(ui);
    ui->SetownerPanel(this);

    return t;
  }


  template <typename T>
  T* GetUI(std::wstring name)
  {
    return dynamic_cast<T*>(uiMap[name]);
  }


  void ShowUI(std::wstring name) { uiMap[name]->Activate(); }
  void HideUI(std::wstring name) { uiMap[name]->Deactivate(); }

  void RemoveUI(std::wstring name)
  {
    UIElement* ui = uiMap[name];
    uiList.remove(ui);
    uiMap.erase(name);
    delete ui;
    ui = nullptr;
  }
};
