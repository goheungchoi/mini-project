#pragma once

#include "GameFramework/Common.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"

class GameObject
{
protected:
  class World* world = nullptr;

public:
  std::string tag;
  std::string name;
	
  GameObject* parent;
  std::list<GameObject*> childrens;

	using ComponentRegistry = std::unordered_map<std::type_index, class ComponentBase*>;
  ComponentRegistry components;

	/* Properties */
  bool bNeedTransformUpdate{false};
	TransformComponent* transform;

  bool bShouldActivate;
  bool bShouldDeactivate;
  bool bShouldDestroy;

	EStatus status{EStatus_Awake};
  bool isActive{false};

	GameObject(class World* world) : world{world} {
    transform = CreateComponent<TransformComponent>();
    status = EStatus_Awake;
    bShouldActivate = true;
    bShouldDeactivate = false;
    bShouldDestroy = false;
	}

  virtual ~GameObject() = default;

  void BeginDestroy()
  {
    status = EStatus_Cleanup;

    // Reset to initial states
    bNeedTransformUpdate = false;
    transform = nullptr;
    bShouldActivate = false;
    bShouldDeactivate = false;
    bShouldDestroy = false;

    // Clean up components.
    for (auto [type_id, pComp] : components)
    {
      delete pComp;
    }
    components.clear();
  }

  void FinishDestroy() { status = EStatus_Destroyed; }

	void SetWorld(class World* world) { this->world = world; }
  class World* GetWorld() const { return world; }

  void SetGameObjectTag(const std::string& tag);
  const std::string& GetTag() const { return tag; }

	void SetName(const std::string& name);
  const std::string& GetName() const { return name; }

  /* Game Object Transform */

  const XMMATRIX& GetLocalTransform() const
  {
    return transform->localTransform;
  }
  const XMMATRIX& GetWorldTransform() const 
  { 
    return transform->globalTransform; 
  }

  /* Game Object Hierarchy */

  void AddChild(GameObject* gameObject)
  {
    gameObject->parent = this;
    childrens.push_back(gameObject);
    transform->AddChild(gameObject->transform);
  }
  void RemoveChild(GameObject* gameObject)
  {
    gameObject->parent = nullptr;
    childrens.remove(gameObject);
    transform->RemoveChild(gameObject->transform);
  }

  /* Component Management */

	template <ComponentType T>
    requires(!std::same_as<T, MeshComponent> &&
             !std::same_as<T, SkeletalMeshComponent>)
  T* CreateComponent()
  {
    T* component = new T(this);
    components.insert({std::type_index(typeid(T)), component});
    return component;
  }

  template <ComponentType T>
    requires(std::same_as<T, MeshComponent> || std::same_as<T, SkeletalMeshComponent>)
  T* CreateComponent()
  {
    T* meshComp = new T(this); 
    components.insert({std::type_index(typeid(T)), meshComp});
    return meshComp;
  }

	template <ComponentType T>
  T* GetComponent()
  {
    auto it = components.find(std::type_index(typeid(T)));
    return (it == components.end()) ? nullptr : (T*)it->second;
  }

  template <ComponentType T>
  bool HasComponent()
  {
    auto it = components.find(std::type_index(typeid(T)));
    return it != components.end();
  }

  template <ComponentType T>
  void RemoveComponent(){
    if (auto it = components.find(std::type_index(typeid(T)));
        it != components.end())
    {
      components.erase(it);
    }
  }

  GameObject* Clone();

	// Transformation 
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  virtual void OnAwake() {}
  virtual void OnActivated() {}

  void Activate() { 
    if (status == EStatus_Active)
      return;

    bShouldActivate = true; 
  }
  void Deactivate() { 
    if (status == EStatus_Inactive)
      return;

    bShouldDeactivate = true; 
  }
  void Destroy() { 
    if (status == EStatus_Cleanup || status == EStatus_Destroyed)
      return;

    bShouldDestroy = true; 
  }

	virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}

private:
  void RegisterMeshComponentToWorld(MeshComponent* meshComp);
  void RegisterMeshComponentToWorld(SkeletalMeshComponent* skeletalMeshComp);
};

template <class T>
concept GameObjectType = std::is_base_of<GameObject, T>::value;
