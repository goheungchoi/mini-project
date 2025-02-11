#include "GameFramework/Level/Level.h"

#include "GameFramework/World/World.h"

void Level::DestroyLevel()
{
  for (GameObject* gameObject : gameObjects)
  {
    for (auto [type_id, pComp] : gameObject->components)
    {
      delete pComp;
    }
    gameObject->components.clear();

    delete gameObject;
  }
  gameObjects.clear();
  gameObjectTagMap.clear();
  gameObjectTypeMap.clear();
  gameObjectNameMap.clear();

  world->_renderer->DestroyMesh();
}
