#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"

#include "ResourceManager/ResourceManager.h"

class Level
{
  using GameObjectTagMap = std::unordered_map<std::string, GameObject*>;
  using GameObjectTypeMap = std::unordered_multimap<std::type_index, GameObject*>;

	std::string name;
  class World* world;

protected:
  GameObjectTagMap gameObjectTagMap;
  GameObjectTypeMap gameObjectTypeMap;

public:
  Level(class World* world, const std::string& name);
  virtual ~Level();

	template <GameObjectType T>
  T* CreateGameObject()
  {
    T* newGameObject = new T(world);
    gameObjects.push_back(newGameObject);
    gameObjectTypeMap.insert({std::type_index(typeid(T)), newGameObject});
    return newGameObject;
  }

	template <GameObjectType T>
  T* CreateGameObjectFromModel(const std::string& path)
  {
    ModelHandle modelHandle = LoadModel(path);
    if (modelHandle.IsInvalid())
    {
      throw std::runtime_error("Can't find a model: "s + path);
    }

    const ModelData& data = AccessModelData(modelHandle);
    bool isSkeleton = !data.skeleton.IsInvalid();

    // Root game object
    T* rootGameNode = CreateGameObject<T>();
    rootGameNode->SetName(data.nodes[0].name);

    // Create a right-sibling tree of the game objects.
    std::vector<GameObject*> gameObjNodes;
    gameObjNodes.resize(data.nodes.size());
    gameObjNodes[0] = rootGameNode;

    if (isSkeleton)
    {
      const SkeletonData& skeleton = AccessSkeletonData(data.skeleton);

      std::unordered_map<BoneId, GameObject*> gameObjectBoneId;
      std::vector<std::pair<SkeletalMeshComponent*, BoneId>>
          skeletalMeshRootBonePair;

      // Create node game objects
      for (int i = 1; i < data.nodes.size(); ++i)
      {
        // Create a node game object.
        GameObject* newNode = CreateGameObject<GameObject>();
        newNode->SetName(data.nodes[i].name);
        newNode->transform->globalTransform = data.nodes[i].transform;

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
          }
          else
          {
            // Skeletal mesh
            SkeletalMeshComponent* skeletalMeshComponent =
                newNode->CreateComponent<SkeletalMeshComponent>();
            skeletalMeshComponent->SetHandle(meshHandle);
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
      // Create node game objects.
      for (int i = 1; i < data.nodes.size(); ++i)
      {
        GameObject* newNode = CreateGameObject<GameObject>();
        gameObjNodes[i] = newNode;

        for (MeshHandle meshHandle : data.nodes[i].meshes)
        {
          const MeshData& meshData = AccessMeshData(meshHandle);

          GameObject* meshNode = CreateGameObject<GameObject>();
          meshNode->SetName(meshData.name);

          MeshComponent* meshComponent =
              meshNode->CreateComponent<MeshComponent>();
          meshComponent->SetHandle(meshHandle);

          newNode->AddChild(meshNode);
        }

        gameObjNodes[data.nodes[i].parent]->AddChild(newNode);
      }
    }

    return rootGameNode;
	}

  template<GameObjectType T>
  T* FindGameObjectByType()
  {
    auto it = gameObjectTypeMap.find(std::type_index(typeid(T)));
    if (it == gameObjectTypeMap.end())
      return nullptr;
    return static_cast<T*>(it->second);
  }

  template<GameObjectType T>
  std::vector<T*> FindAllGameObjectByType()
  {
    std::vector<T*> res;
    auto range = gameObjectTypeMap.equal_range(std::type_index(typeid(T)));
    for (auto it = range.first; it != range.second; ++it)
    {
      res.push_back(static_cast<T*>(it->second));
    }
    return res;
  }
  
  GameObject* GetGameObjectByTag(const std::string& tag) {
    auto it = gameObjectTagMap.find(tag);
    if (it == gameObjectTagMap.end())
      return nullptr;
    return it->second;
  }

  std::vector<GameObject*> FindAllGameObjectsByTag(const std::string& tag)
  {
    std::vector<GameObject*> res;
    auto range = gameObjectTagMap.equal_range(tag);
    for (auto it = range.first; it != range.second; ++it)
    {
      res.push_back(it->second);
    }
    return res;
  }

public:
  std::list<GameObject*> gameObjects;

  class World* GetWorld() { return world; }

  virtual void Enter() { BeginPlay(); };
  virtual void Exit() {};
  virtual void Clear() {};

  virtual void BeginPlay();
  virtual void FixedUpdate(float _fixedRate);
  virtual void Destroy();
  virtual void CleanUp();
  virtual void PreUpdate(float _dt);
  virtual void Update(float _dt);
  virtual void PostUpdate(float _dt);
  virtual void Render();
};

template <class T>
concept LevelType = std::is_base_of<Level, T>::value;
