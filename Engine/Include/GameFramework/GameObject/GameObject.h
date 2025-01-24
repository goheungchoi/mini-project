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

  const XMVECTOR& GetLocalRight() const { return transform->GetLocalRight(); }
  const XMVECTOR& GetLocalUp() const { return transform->GetLocalUp(); }
  const XMVECTOR& GetLocalFront() const { return transform->GetLocalFront(); }

  const XMVECTOR& GetGlobalRight() const { return transform->GetGlobalRight(); }
  const XMVECTOR& GetGlobalUp() const { return transform->GetGlobalUp(); }
  const XMVECTOR& GetGlobalFront() const { return transform->GetGlobalFront(); }

  const void SetScaling(float scaling) { transform->SetScaling(scaling); }
  const void SetScaling(float x, float y, float z)
  {
    transform->SetScaling(x, y, z);
  }
  const void SetScaling(XMVECTOR scaling) { transform->SetScaling(scaling); }
  void SetRotation(float pitch, float yaw, float roll)
  {
    transform->SetRotation(pitch, yaw, roll);
  }
  void SetRotation(XMVECTOR euler) { transform->SetRotation(euler);
  }
  void SetRotationAroundAxis(XMVECTOR axis, float angle)
  {
    transform->SetRotationAroundAxis(axis, angle);
  }
  void SetTranslation(float x, float y, float z)
  {
    transform->SetTranslation(x, y, z);
  }
  void SetTranslation(XMVECTOR translation)
  {
    transform->SetTranslation(translation);
  }

  void Scale(float scaling)
  { transform->Scale(scaling);
  }
  void Scale(float x, float y, float z)
  {
    transform->Scale(x, y, z);
  }
  void Scale(XMVECTOR scaling)
  {
    transform->Scale(scaling);
  }

  void Rotate(float pitch, float yaw, float roll)
  {
    transform->Rotate(pitch, yaw, roll);
  }
  void Rotate(XMVECTOR euler) { transform->Rotate(euler); }
  void RotateAroundXAxis(float angle) { transform->RotateAroundXAxis(angle); }
  void RotateAroundYAxis(float angle) { transform->RotateAroundYAxis(angle); }
  void RotateAroundZAxis(float angle) { transform->RotateAroundZAxis(angle); }
  void RotateAroundAxis(XMVECTOR axis, float angle)
  {
    transform->RotateAroundAxis(axis, angle);
  }
  void RotateToward(XMVECTOR target, float maxAngleStep) {
    transform->RotateToward(target, maxAngleStep);
  }

  void Translate(float x, float y, float z) { transform->Translate(x, y, z); }
  void Translate(XMVECTOR translation) { transform->Translate(translation); }

  void SetLocalTransform(XMMATRIX transform) { this->transform->SetLocalTransform(transform); }

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
  virtual void OnRender() {}

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
