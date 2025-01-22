#pragma once

#include "GameFramework/Systems/RenderSystem.h"

#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/Components/MeshComponent.h"

void RenderSystem::Execute(const std::list<GameObject*>& gameObjects) {
	


	for (GameObject* gameObject : gameObjects)
  {
    auto* transformComponent = gameObject->GetComponent<TransformComponent>();
		auto* meshComponent = gameObject->GetComponent<MeshComponent>();
    if (meshComponent)
    {
      const XMMATRIX& transform = gameObject->GetWorldTransform();
      MeshHandle meshHandle = meshComponent->GetHandle();

			_renderer->BeginDraw(meshHandle, transform);
      _renderer->DrawMesh(meshHandle);
		}
	}


}
