#pragma once

#include "Core/Input/InputSystem.h"
#include "ResourceManager/ResourceManager.h"

#include "GameFramework/Common.h"
#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/Components/RigidbodyComponent.h"
#include "GameFramework/Components/BillboardComponent.h"

class GameObject
{
protected:
  class World* world = nullptr;

public:
  std::type_index typeIndex{typeid(void)};
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

	GameObject(class World* world) : world{world}
  {
    transform = CreateComponent<TransformComponent>();
    status = EStatus_Awake;
    bShouldActivate = true;
    bShouldDeactivate = false;
    bShouldDestroy = false;
  }

  virtual ~GameObject() = default;

	/* Game Object Hierarchy */

  void AddChildGameObject(GameObject* gameObject)
  {
    gameObject->parent = this;
    childrens.push_back(gameObject);
    transform->AddChild(gameObject->transform);
  }
  void RemoveChildGameObject(GameObject* gameObject)
  {
    gameObject->parent = nullptr;
    childrens.remove(gameObject);
    transform->RemoveChild(gameObject->transform);
  }

	void DetachFromParentTransform() { transform->DetachFromParent(); }

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

  void FinishDestroy() { 
		status = EStatus_Destroyed;
		
		// Clean up components.
    for (auto [type_id, pComp] : components)
    {
      delete pComp;
    }
    components.clear();

		transform = nullptr;
	}

	void SetWorld(class World* world) { this->world = world; }
  class World* GetWorld() const { return world; }

	void SetName(const std::string& name);
  const std::string& GetName() const { return name; }

  void SetGameObjectTag(const std::string& tag);
  const std::string& GetGameObjectTag() const { return tag; }


	void RemoveFromTypeRegistration();


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
  void SetRotation(XMVECTOR euler) { transform->SetRotation(euler); }

	void SetRotationAroundXAxis(float angle)
  {
    transform->SetRotationAroundXAxis(angle);
  }
  void SetRotationAroundYAxis(float angle)
  {
    transform->SetRotationAroundYAxis(angle);
  }
  void SetRotationAroundZAxis(float angle)
  {
    transform->SetRotationAroundZAxis(angle);
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
  /*void RotateToward(XMVECTOR target, float maxAngleStep) {
    transform->RotateToward(target, maxAngleStep);
  }*/

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

  /* Component Management */

	template <ComponentType T>
  T* CreateComponent()
  {
    T* component = new T(this);
    components.insert({std::type_index(typeid(T)), component});
    return component;
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

	GameObject* FindChildGameObject(const std::string& name) {
    std::stack<GameObject*> st;
    st.push(this);

		while (!st.empty())
    {
      GameObject* object = st.top();
      st.pop();

			if (object->name == name)
      {
        return object;
			}

			for (auto* child : object->childrens)
      {
        st.push(child);
			}
		}
    return nullptr;
	}

  void SetVisible();
  void SetInvisible();

	// TODO:
  // GameObject* Clone();

	// Interaction 
	virtual void OnBeginOverlap(GameObject* other)
  {
    GetComponent<RigidbodyComponent>()->SetIsOverlapping(true);
  }
  virtual void OnEndOverlap(GameObject* other)
  {
    GetComponent<RigidbodyComponent>()->SetIsOverlapping(false);
  }

  virtual void OnHover() {};
  virtual void OnLeftClick() {};
  virtual void OnRightClick() {};

  // Game loop events
  virtual void OnAwake() {}
  virtual void OnActivated() {}

	virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}
  
private:
  void RegisterMeshComponentToWorld(MeshComponent* meshComp);
  void RegisterMeshComponentToWorld(SkeletalMeshComponent* skeletalMeshComp);
};

template <class T>
concept GameObjectType = std::is_base_of<GameObject, T>::value;
