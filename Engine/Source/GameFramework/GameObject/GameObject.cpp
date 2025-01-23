#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/World/World.h"

void GameObject::SetGameObjectTag(const std::string& tag)
{
  this->tag = tag;
  world->RegisterGameObjectTag(this);
}

void GameObject::SetName(const std::string& name)
{
  this->name = name;
  world->RegisterGameObjectName(this);
}

GameObject* GameObject::Clone()
{
  /* TODO: */
  return nullptr;
}

void GameObject::RegisterMeshComponentToWorld(MeshComponent* meshComp) {
  world->RegisterMeshComponent(meshComp);
}

void GameObject::RegisterMeshComponentToWorld(
    SkeletalMeshComponent* skeletalMeshComp)
{
  world->RegisterMeshComponent(skeletalMeshComp);
}
