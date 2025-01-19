#pragma once

#include "GameFramework/Common.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"

#include "GameFramework/Components/Colliders/ColliderComponent.h"
#include "GameFramework/Components/Colliders/BoxColliderComponent.h"
#include "GameFramework/Components/Colliders/CapsuleColliderComponent.h"
#include "GameFramework/Components/Colliders/MeshColliderComponent.h"
#include "GameFramework/Components/Colliders/SphereColliderComponent.h"

#include "GameFramework/Types/OverlapInfo.h"

class GameObject
{
protected:
  class World* world = nullptr;

public:
  std::string name;
	
  GameObject* parent;
  std::vector<GameObject*> childrens;

	using ComponentRegistry = std::unordered_map<std::type_index, class ComponentBase*>;
  ComponentRegistry components;

	/* Properties */
  bool bNeedTransformUpdate{false};
	class TransformComponent* transform{nullptr};

	EStatus status{EStatus_Awake};
  bool isActive{false};

	GameObject(class World* world) : world{world} {
    CreateComponent<TransformComponent>();
	}

	void SetWorld(class World* world) { this->world = world; }
  class World* GetWorld() { return world; }

	void SetName(const std::string& name) { this->name = name; }
  const std::string& GetName() const { return name; }

	void AddChild(GameObject* gameObject);
  void RemoveChild(GameObject* gameObject);

	void SetActive(bool active)
  {
    isActive = active;
    status = active ? EStatus_Active : EStatus_Inactive;
  }

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

	// Transformation 


	// Collision Event
  void NotifyBeginOverlap(GameObject* other, const OverlapInfo& overlap)
  {
    OnBeginOverlap(other, overlap);
  }
  void NotifyOverlap(GameObject* other, const OverlapInfo& overlap)
  {
    OnOverlap(other, overlap);
  }
  void NotifyEndOverlap(GameObject* other, const OverlapInfo& overlap)
  {
    OnEndOverlap(other, overlap);
  }
  void NotifyBlockingHit(ColliderComponent* myComp,
                         ColliderComponent* otherComp, bool bSelfMoved,
                         const HitResult& hitResult)
  {
		// TODO:
    // OnHit(myComp, otherComp, bSelfMoved, hitResult);
  }

	/**
   * @brief ClickComponent Evnet
   */
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

		/**
   * @brief
   * @param other
   * @param overlap
   */
  virtual void OnBeginOverlap(GameObject* other, const OverlapInfo& overlap) {}

  /**
   * @brief
   * @param other
   * @param overlap
   */
  virtual void OnOverlap(GameObject* other, const OverlapInfo& overlap) {}

  /**
   * @brief
   * @param other
   * @param overlap
   */
  virtual void OnEndOverlap(GameObject* other, const OverlapInfo& overlap) {}

	virtual void FixedUpdate(float fixedRate);
  virtual void PreUpdate(float dt);
  virtual void Update(float dt);
  virtual void PostUpdate(float dt);
};

template <class T>
concept GameObjectType = std::is_base_of<GameObject, T>::value;
