#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/World/World.h"


void GameObject::SetName(const std::string& name)
{
  if (name.empty())
  {
    world->UnregisterGameObjectName(this);
    return;
  }

  if (!this->tag.empty())
  {
    world->UnregisterGameObjectName(this);
  }
  this->name = name;
  world->RegisterGameObjectName(this);
}

void GameObject::SetGameObjectTag(const std::string& tag)
{
	// If empty tag is inputted
  if (tag.empty())
  {
		// Reset the game object tag
    world->UnregisterGameObjectTag(this);
    return;
	}

	// If the current tag is not empty.
  if (!this->tag.empty())
  {
		// Unregister the original game object tag first.
    world->UnregisterGameObjectTag(this);
	}
	// Change tag first
  this->tag = tag;
	// Register it to the world.
  world->RegisterGameObjectTag(this);
}

void GameObject::RemoveFromTypeRegistration() {
  world->UnregisterGameObjectType(this);
}

//GameObject* GameObject::Clone()
//{
//  /* TODO: */
//  return nullptr;
//}

void GameObject::SetVisible() {
  auto* meshComp = GetComponent<MeshComponent>();
  if (meshComp)
  {
    meshComp->SetVisible(true);
  }

  auto* skelMeshComp = GetComponent<SkeletalMeshComponent>();
  if (skelMeshComp) {
    skelMeshComp->SetVisible(true);
  }

  auto* bbComp = GetComponent<BillboardComponent>();
  if (bbComp)
  {
    bbComp->isVisible = true;
  }
}

void GameObject::SetInvisible() {
  auto* meshComp = GetComponent<MeshComponent>();
  if (meshComp)
  {
    meshComp->SetVisible(false);
  }

  auto* skelMeshComp = GetComponent<SkeletalMeshComponent>();
  if (skelMeshComp)
  {
    skelMeshComp->SetVisible(false);
  }

  auto* bbComp = GetComponent<BillboardComponent>();
  if (bbComp)
  {
    bbComp->isVisible = false;
  }
}

void GameObject::SetOutlineColor(Color color) 
{
  auto* meshComp = GetComponent<MeshComponent>();
  if (meshComp)
  {
    meshComp->outlineColor = color;
  }

  auto* skelMeshComp = GetComponent<SkeletalMeshComponent>();
  if (skelMeshComp)
  {
    skelMeshComp->outlineColor = color;
  }

  for (GameObject* gameObject : childrens)
  {
    auto* meshComp = gameObject->GetComponent<MeshComponent>();
    if (meshComp)
    {
      meshComp->outlineColor = color;
    }

    auto* skelMeshComp = gameObject->GetComponent<SkeletalMeshComponent>();
    if (skelMeshComp)
    {
      skelMeshComp->outlineColor = color;
    }
  }
}

void GameObject::ShowOutline()
{
  auto* meshComp = GetComponent<MeshComponent>();
  if (meshComp)
  {
    meshComp->ShowOutline();
  }

  auto* skelMeshComp = GetComponent<SkeletalMeshComponent>();
  if (skelMeshComp)
  {
    skelMeshComp->ShowOutline();
  }

  for (GameObject* gameObject : childrens)
  {
    auto* meshComp = gameObject->GetComponent<MeshComponent>();
    if (meshComp)
    {
      meshComp->ShowOutline();
    }

    auto* skelMeshComp = gameObject->GetComponent<SkeletalMeshComponent>();
    if (skelMeshComp)
    {
      skelMeshComp->ShowOutline();
    }
  }
}

void GameObject::HideOutline() {
  auto* meshComp = GetComponent<MeshComponent>();
  if (meshComp)
  {
    meshComp->HideOutline();
  }

  auto* skelMeshComp = GetComponent<SkeletalMeshComponent>();
  if (skelMeshComp)
  {
    skelMeshComp->HideOutline();
  }

  for (GameObject* gameObject : childrens)
  {
    auto* meshComp = gameObject->GetComponent<MeshComponent>();
    if (meshComp)
    {
      meshComp->HideOutline();
    }

    auto* skelMeshComp = gameObject->GetComponent<SkeletalMeshComponent>();
    if (skelMeshComp)
    {
      skelMeshComp->HideOutline();
    }
  }
}

void GameObject::RegisterMeshComponentToWorld(MeshComponent* meshComp)
{
  world->RegisterMeshComponent(meshComp);
}

void GameObject::RegisterMeshComponentToWorld(
    SkeletalMeshComponent* skeletalMeshComp)
{
  world->RegisterMeshComponent(skeletalMeshComp);
}
