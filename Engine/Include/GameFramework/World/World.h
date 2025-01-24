#pragma once

#include "Core/Common.h"

#include "Renderer/IRenderer.h"

#include "Core/Async/Async.h"

#include "GameFramework/Level/Level.h"
#include "../../PhyjixEngine/pch.h"

class World
{
public:

  IRenderer* _renderer{nullptr};

  // UI System
  // class Canvas* _canvas = nullptr;

  // EffectSystem
  // EffectSystem _effectSystem;

  // Collision System
  IPhyjixEngine* _phyjixEngine;
  IPhyjixWorld* _phyjixWorld;








  class Camera* _defaultCamera{nullptr};

  World() = default;
  virtual ~World() = default;

  void Initialize(HWND hwnd, const std::wstring& title);
  
public:
  std::atomic_bool readyToChangeLevel{true};
  std::atomic_bool changingLevel{false};
  std::unordered_map<std::string, Level*> _levelMap;
  std::unordered_map<std::string, std::future<bool>> _levelPreparedMap;

  Level* _preparingLevel{nullptr};

  Level* _currentLevel{nullptr};
  
  bool bCameraMove{false};
  class Camera* mainCamera{nullptr};

  static World* CreateWorld(HWND hwnd, const std::wstring& title);

  /* Asynchronously load the next level. */
	void PrepareChangeLevel(const std::string& levelName);
  /* Return true if the next level is loaded. */
  bool IsLevelChangeReady();
  /* Actually performs the level transition. */
  void CommitLevelChange();

  void AddLevel(Level* level);
  Level* GetCurrentLevel() { return _currentLevel; }

  /**
   * @brief Set the main camera.
   * @param camera If nullptr, the default camera is bound. Otherwise, the
   * inputted camera is used.
   */
  void SetMainCamera(class Camera* camera);
  /**
   * @brief Stop the camera movement.
   * @param fixed 
   */
  void StopCameraMovement(bool fixed);

  XMVECTOR ScreenToWorldPosition(XMVECTOR position);

  /* Game Object Management Functions */

  template <GameObjectType T>
  T* CreateGameObject()
  {
    T* newGameObject = new T(this);
    _currentLevel->AddGameObject<T>(newGameObject);
    return newGameObject;
  }

  // template <GameObjectType T>
  GameObject* CreateGameObjectFromModel(ModelHandle modelHandle)
  {
    if (modelHandle.IsInvalid())
    {
      throw std::runtime_error("Invalid model handle!");
    }

    // TODO: Fix the logic!
    const ModelData& data = AccessModelData(modelHandle);
    const bool isSkeleton = !data.skeleton.IsInvalid();

    // Root game object
    GameObject* rootGameNode = CreateGameObject<GameObject>();
    rootGameNode->SetName(data.nodes[0].name);
    rootGameNode->transform->globalTransform = data.nodes[0].transform;

    // Create a right-sibling tree of the game objects.
    std::vector<GameObject*> gameObjNodes;
    gameObjNodes.resize(data.nodes.size());
    gameObjNodes[0] = rootGameNode;

    if (isSkeleton)
    {
      // Fetch the skeleton data.
      const SkeletonData& skeleton = AccessSkeletonData(data.skeleton);

      // Map GameObjects to boneIds.
      std::unordered_map<BoneId, GameObject*> gameObjectBoneId;
      // Find the root bone of the skeletal mesh components.
      std::vector<std::pair<SkeletalMeshComponent*, BoneId>>
          skeletalMeshRootBonePair;

      // Create node game objects
      for (int i = 1; i < data.nodes.size(); ++i)
      {
        // Create a node game object.
        GameObject* newNode = CreateGameObject<GameObject>();
        newNode->SetName(data.nodes[i].name);
        newNode->SetLocalTransform(data.nodes[i].transform);

        gameObjNodes[i] = newNode;
        gameObjectBoneId[skeleton.nodes[i].boneId] = newNode;

        // Create game objects with mesh components.
        for (MeshHandle meshHandle : data.nodes[i].meshes)
        {
          const MeshData& meshData = AccessMeshData(meshHandle);

          if (meshData.bones.empty())
          {
            // Static mesh
            MeshComponent* meshComponent =
                newNode->CreateComponent<MeshComponent>();
            meshComponent->SetHandle(meshHandle);
            meshComponent->RegisterMeshToWorld();
          }
          else
          {
            // Skeletal mesh
            SkeletalMeshComponent* skeletalMeshComponent =
                newNode->CreateComponent<SkeletalMeshComponent>();
            skeletalMeshComponent->SetHandle(meshHandle);
            skeletalMeshComponent->RegisterMeshToWorld();

            BoneId rootBoneNode = meshData.bones.front().id;
            skeletalMeshRootBonePair.push_back(
                {skeletalMeshComponent, rootBoneNode});
          }
        }

        gameObjNodes[data.nodes[i].parent]->AddChild(newNode);
      }

      // Bine the root bones to the skeletal mesh.
      for (auto [skeletalMesh, boneId] : skeletalMeshRootBonePair)
      {
        auto* gameObject = gameObjectBoneId[boneId];
        skeletalMesh->SetRootTransform(gameObject->transform);
      }
    }
    else
    {
      // TODO: Debugging
      // Create node game objects.
      for (int i = 0; i < data.nodes.size(); ++i)
      {
        GameObject* newNode;
        if (i > 0)
        {
          newNode = CreateGameObject<GameObject>();
          gameObjNodes[i] = newNode;
        }
        else
        {
          newNode = rootGameNode;
        }

        for (MeshHandle meshHandle : data.nodes[i].meshes)
        {
          const MeshData& meshData = AccessMeshData(meshHandle);

          GameObject* meshNode = CreateGameObject<GameObject>();
          meshNode->SetName(meshData.name);
          meshNode->transform->globalTransform = data.nodes[i].transform;

          MeshComponent* meshComponent =
              meshNode->CreateComponent<MeshComponent>();
          meshComponent->SetHandle(meshHandle);
          meshComponent->RegisterMeshToWorld();

          newNode->AddChild(meshNode);
        }

        if (data.nodes[i].parent >= 0)
          gameObjNodes[i]->AddChild(newNode);
      }
    }

    return rootGameNode;
  }

  void RegisterGameObjectName(GameObject* gameObject);
  void UnregisterGameObjectName(GameObject* gameObject);
  void RegisterGameObjectTag(GameObject* gameObject);
  void UnregisterGameObjectTag(GameObject* gameObject);

  void RegisterMeshComponent(class MeshComponent* meshComp);
  void RegisterMeshComponent(class SkeletalMeshComponent* skeletalMeshComp);

  // bool CheckComponentOverlapMulti(
  //	std::vector<OverlapResult>& outOverlapResults,
  //	class PrimitiveComponent* primComp,
  //	const Math::Vector2& pos,
  //	const Math::Matrix& rotation);

  // bool CheckComponentOverlapMultiByChannel(
  //	std::vector<OverlapResult>& outOverlapResults,
  //	class PrimitiveComponent* primComp,
  //	const Math::Vector2& pos,
  //	const Math::Matrix& rotation,
  //	ECollisionChannel channel);

  // bool CheckComponentSweepMulti(
  //	std::vector<HitResult>& outHitResults,
  //	class PrimitiveComponent* primComp,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	const Math::Matrix& rotation);

  // bool CheckComponentSweepMultiByChannel(
  //	std::vector<HitResult>& outHitResults,
  //	class PrimitiveComponent* primComp,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	const Math::Matrix& rotation,
  //	ECollisionChannel channel);

  // bool LineTraceMultiByChannel(
  //	std::vector<HitResult>& outHitResults,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	ECollisionChannel channel,
  //	const CollisionProperty& collisionProperty) const {
  //	// TODO
  //	return false;
  // }

  // bool LineTraceSingleByChannel(
  //	HitResult& outHitResults,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	ECollisionChannel channel,
  //	const CollisionProperty& collisionProperty) const {
  //	// TODO
  //	return false;
  // }

  // bool OverlapMultiByChannel(
  //	std::vector<OverlapResult>& outOverlapResults,
  //	const Math::Vector2& pos,
  //	ECollisionChannel channel,
  //	const CollisionShape& collisionShape,
  //	const CollisionProperty& collisionProperty) const {
  //	// TODO
  //	return false;
  // }

  // bool SweepMultiByChannel(
  //	std::vector<HitResult>& outHitResults,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	ECollisionChannel channel,
  //	const CollisionShape& collisionShape,
  //	const CollisionProperty& collisionProperty) const {
  //	// TODO
  //	return false;
  // }

  // bool SweepSingleByChannel(
  //	HitResult& outHitResults,
  //	const Math::Vector2& start,
  //	const Math::Vector2& end,
  //	ECollisionChannel channel,
  //	const CollisionShape& collisionShape,
  //	const CollisionProperty& collisionProperty) const {
  //	// TODO
  //	return false;
  // }





  /* Game Loop Flow */
  
  void InitialStage();

  void FixedUpdate(float fixedRate);
  void PhysicsUpdate(float fixedRate);

  void ProcessInput(float dt);
  void PreUpdate(float dt);
  void Update(float dt);
  void AnimationUpdate(float dt);
  void PostUpdate(float dt);

  void RenderGameObjects();
  void RenderUI();

  void CleanupStage();

private:
  void UpdateGameObjectHierarchy(GameObject* gameObject,
                                 std::function<void(GameObject*)> func);
};
