#include "GameFramework/World/World.h"

#include "Renderer/DX11/DX11Renderer.h"

#include "Core/Camera/Camera.h"
#include "Core/Common.h"
#include "Core/Input/InputSystem.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/RigidbodyComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"

#include "Interface.h"
#include "PhyjixEngine.h"

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif

#include "GameFramework/UI/Canvas/Canvas.h"

void World::Initialize(HWND hwnd, const std::wstring& title)
{
  _renderer = new DX11Renderer();
  _renderer->Init_Win32(kScreenWidth, kScreenHeight, nullptr, &hwnd);
  _renderer->CreateSkyBox(
      "Textures/BakerEnv.dds", "Textures/BakerSpecularBRDF_LUT.dds",
      "Textures/BakerDiffuseIrradiance.dds", "Textures/BakerSpecularIBL.dds");

  _defaultCamera = new Camera(kScreenWidth, kScreenHeight, XM_PIDIV4);

  _phyjixEngine = new PhyjixEngine();
  _phyjixEngine->Initialize();
  _phyjixWorld = _phyjixEngine->CreateWorld();
  _phyjixWorld->CreateDefaultGround();

  SetMainCamera(_defaultCamera);

#ifdef USED2D
  _canvas = new Canvas(this);
#endif // USED2D
}

World* World::CreateWorld(HWND hwnd, const std::wstring& title)
{
  World* newWorld = new World();
  newWorld->Initialize(hwnd, title);

  return newWorld;
}

void World::PrepareChangeLevel(const std::string& levelName)
{
  /* TODO: */
  if (auto it = _levelMap.find(levelName); it != _levelMap.end())
  {
    if (_preparingLevel)
    {
      throw std::runtime_error("There is already a preparing level.");
    }

    _preparingLevel = it->second;
    bReadyToChangeLevel = false;

    // Async level preparation.
    async::executor.silent_async("prepare level", [this]() {
      _preparingLevel->PrepareLevel();
      bReadyToChangeLevel = true;
    });
  }
  else
  {
    throw std::runtime_error("Level is not found: " + levelName);
  }
}

bool World::IsLevelChangeReady()
{
  if (!_preparingLevel)
    return false;

  return bReadyToChangeLevel;
}

void World::CommitLevelChange()
{
  // Check if any level is being prepared to be executed.
  if (!_preparingLevel)
  {
    throw std::runtime_error("There no prepared level exists!");
  }

  // Wait until the level asset loading is done.
  while (!bReadyToChangeLevel) {}
  
  
  bChangingLevel = true;

  if (_currentLevel)
  {
    _currentLevel->DestroyLevel();

    auto& tmp = _currentLevel;
    async::executor.silent_async([tmp]() {
      tmp->CleanupLevel();
    });

    rigidBodyComponents.clear();

#ifdef USED2D
    delete _canvas;
    _canvas = new Canvas(this);
#endif // USED2D
  }

  // Change the current level, and begin the level.
  _currentLevel = _preparingLevel;
  _currentLevel->BeginLevel();
  _phyjixWorld->CreateRay(
      mainCamera->GetPosition(),
      Vector2(INPUT.GetCurrMouseState().x, INPUT.GetCurrMouseState().y),
      mainCamera->GetViewTransform(), mainCamera->GetProjectionMatrix(),
      Vector2(kScreenWidth, kScreenHeight));

  _preparingLevel = nullptr;
  bReadyToChangeLevel = false;

  bChangingLevel = false;
}

void World::AddLevel(Level* level)
{
  level->world = this;
  _levelMap[level->name] = level;
}

void World::SetMainCamera(Camera* camera)
{
  if (camera)
    mainCamera = camera;
  else
    mainCamera = _defaultCamera;
}

void World::StopCameraMovement(bool fixed)
{
  bCameraMove = !fixed;
}

Ray World::GetScreenCursorRay(XMVECTOR screenPos)
{
  // Convert screen coordinates to normalized device coordinates (NDC)
  float x = (2.0f * XMVectorGetX(screenPos)) / kScreenWidth - 1.0f;
  float y = 1.0f - (2.0f * XMVectorGetY(screenPos)) / kScreenHeight;

  // Near plane (z = 0) and far plane (z = 1) in NDC
  XMVECTOR ndcNear = XMVectorSet(x, y, 0.0f, 1.0f);
  XMVECTOR ndcFar = XMVectorSet(x, y, 1.0f, 1.0f);

  // Get the inverse of the combined view-projection matrix
  XMMATRIX projectionMatrix = mainCamera->GetProjectionMatrix();
  XMMATRIX viewMatrix = mainCamera->GetViewTransform();
  XMMATRIX invViewProjMatrix =
      XMMatrixInverse(nullptr, XMMatrixMultiply(viewMatrix, projectionMatrix));

  // Unproject the NDC coordinates to world space
  XMVECTOR worldNear = XMVector3TransformCoord(ndcNear, invViewProjMatrix);
  XMVECTOR worldFar = XMVector3TransformCoord(ndcFar, invViewProjMatrix);

  // Calculate the ray direction
  XMVECTOR rayDirection = XMVector3Normalize(worldFar - worldNear);

  // Create and return the ray
  Ray ray;
  ray.position = worldNear;
  ray.direction = rayDirection;

  return ray;
}

GameObject* World::FindGameObjectByTag(const std::string& tag)
{
  if (!_currentLevel)
    return nullptr;

  auto it = _currentLevel->gameObjectTagMap.find(tag);
  if (it == _currentLevel->gameObjectTagMap.end())
    return nullptr;
  return it->second;
}

std::vector<GameObject*> World::FindAllGameObjectsByTag(const std::string& tag)
{
  if (!_currentLevel)
    return {};

  std::vector<GameObject*> res;
  auto range = _currentLevel->gameObjectTagMap.equal_range(tag);
  for (auto it = range.first; it != range.second; ++it)
  {
    res.push_back(it->second);
  }
  return res;
}

GameObject* World::FindGameObjectByName(const std::string& name)
{
  if (!_currentLevel)
    return nullptr;

  auto it = _currentLevel->gameObjectNameMap.find(name);
  if (it == _currentLevel->gameObjectNameMap.end())
    return nullptr;
  return it->second;
}

std::vector<GameObject*> World::FindAllGameObjectsByName(
    const std::string& name)
{
  if (!_currentLevel)
    return {};

  std::vector<GameObject*> res;
  auto range = _currentLevel->gameObjectNameMap.equal_range(name);
  for (auto it = range.first; it != range.second; ++it)
  {
    res.push_back(it->second);
  }
  return res;
}

void World::RegisterGameObjectName(GameObject* gameObject)
{
  if (!_currentLevel || !gameObject)
    return;

  _currentLevel->gameObjectNameMap.insert({gameObject->name, gameObject});
}

void World::UnregisterGameObjectName(GameObject* gameObject)
{
  if (!_currentLevel || !gameObject)
    return;

  auto it = _currentLevel->gameObjectNameMap.find(gameObject->name);
  while (it != _currentLevel->gameObjectNameMap.end() &&
         it->first == gameObject->name)
  {
    if (it->second == gameObject)
    {
      _currentLevel->gameObjectNameMap.erase(it);
      break;
    }
    ++it;
  }
}

void World::RegisterGameObjectTag(GameObject* gameObject)
{
  if (!_currentLevel || !gameObject)
    return;

  _currentLevel->gameObjectTagMap.insert({gameObject->tag, gameObject});
}

void World::UnregisterGameObjectTag(GameObject* gameObject)
{
  if (!_currentLevel || !gameObject)
    return;

  auto it = _currentLevel->gameObjectTagMap.find(gameObject->tag);
  while (it != _currentLevel->gameObjectTagMap.end() &&
         it->first == gameObject->tag)
  {
    if (it->second == gameObject)
    {
      _currentLevel->gameObjectTagMap.erase(it);
      break;
    }
    ++it;
  }
}

void World::UnregisterGameObjectType(GameObject* gameObject)
{
  if (!_currentLevel || !gameObject)
    return;

  auto it = _currentLevel->gameObjectTypeMap.find(gameObject->typeIndex);
  while (it != _currentLevel->gameObjectTypeMap.end() &&
         it->first == gameObject->typeIndex)
  {
    if (it->second == gameObject)
    {
      _currentLevel->gameObjectTypeMap.erase(it);
      break;
    }
    ++it;
  }
}

void World::RegisterMeshComponent(MeshComponent* meshComp)
{
  if (!meshComp)
    return;

  if (meshComp->bCastShadow)
  {
    for (auto handle : meshComp->GetSubMeshes())
    {
      _renderer->CreateMesh(handle);
      _renderer->AddShadow(handle);
    }
  }
  else
  {
    for (auto handle : meshComp->GetSubMeshes())
    {
      _renderer->CreateMesh(handle);
    }
  }
}

void World::RegisterMeshComponent(SkeletalMeshComponent* skeletalMeshComp)
{
  if (!skeletalMeshComp)
    return;

  _renderer->CreateMesh(skeletalMeshComp->GetHandle());
  if (skeletalMeshComp->bCastShadow)
    _renderer->AddShadow(skeletalMeshComp->GetHandle());
}

void World::RegisterRigidBodyComponent(RigidbodyComponent* rigidBody)
{
  for (auto* other : rigidBodyComponents)
  {
    other->SetCollisionEvent(rigidBody->GetRigidBody(),
                             eCollisionEventType::eOverlapBegin,
                             [=]() { other->BeginOverlap(rigidBody); });
    other->SetCollisionEvent(rigidBody->GetRigidBody(),
                             eCollisionEventType::eOverlapEnd,
                             [=]() { other->EndOverlap(rigidBody); });

    rigidBody->SetCollisionEvent(other->GetRigidBody(),
                                 eCollisionEventType::eOverlapBegin,
                                 [=]() { rigidBody->BeginOverlap(other); });

    rigidBody->SetCollisionEvent(other->GetRigidBody(),
                                 eCollisionEventType::eOverlapEnd,
                                 [=]() { rigidBody->EndOverlap(other); });
  }
  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover,
                               [=]() { rigidBody->Hover(); });

  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eLClick,
                               [=]() { rigidBody->LeftClick(); });

  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eRClick,
                               [=]() { rigidBody->RightClick(); });

  rigidBodyComponents.push_back(rigidBody);
}

void World::UnregisterRigidBodyComponent(RigidbodyComponent* rigidBody)
{
  rigidBody->DisableSimulation();

  auto it = std::remove(rigidBodyComponents.begin(), rigidBodyComponents.end(),
                        rigidBody);
  rigidBodyComponents.erase(it, rigidBodyComponents.end());
  for (auto rigidbody : rigidBodyComponents)
  {
    rigidbody->RemoveCollisionEvent(rigidBody->GetRigidBody());
  }

  _phyjixWorld->RemoveRigidBody(rigidBody->GetRigidBody());
}

void World::InitialStage()
{
  if (!_currentLevel || bChangingLevel)
    return;

  // Mark that it should not change the level while running the game loop.
  bReadyToChangeLevel = false;

  // Awake and activate game objects.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Awake && gameObject->bShouldActivate)
    {
      // Awake the game objects and activate it.
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        if (object->status == EStatus_Awake && object->bShouldActivate)
        {
          object->OnAwake();
          object->OnActivated();
          object->isActive = true;
          object->status = EStatus_Active;
          object->bShouldActivate = false;
        }
      });
    }

    if (gameObject->status == EStatus_Inactive && gameObject->bShouldActivate)
    {
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        if (object->status == EStatus_Inactive && object->bShouldActivate)
        {
          // Activate the game object.
          object->OnActivated();
          object->isActive = true;
          object->status = EStatus_Active;
          object->bShouldActivate = false;
        }
      });
    }
  }

// 여기에 Level에 맞는 canvas를 준비해야 하나??
#ifdef USED2D
  _canvas->BeginLevel();
#endif // USED2D
}

// void World::FixedUpdate(float fixedRate)
//{
//   if (!_currentLevel || bChangingLevel)
//     return;
//
//   for (GameObject* gameObject : _currentLevel->GetGameObjectList())
//   {
//     if (!(gameObject->status == EStatus_Active))
//       continue;
//
//     gameObject->FixedUpdate(fixedRate);
//   }
// }
//
// void World::PhysicsUpdate(float fixedRate) {
//   if (!_currentLevel || bChangingLevel)
//     return;
//
//   // TODO:
// }

void World::ProcessInput(float dt)
{
  InputSystem::GetInstance()->Update(dt);

  if (INPUT.IsKeyDown(MouseState::LB))
  {
    _phyjixWorld->LeftClick();
  }
}

void World::PreUpdate(float dt)
{
  if (!_currentLevel || bChangingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->PreUpdate(dt);
  }
}

void World::Update(float dt)
{
  if (!_currentLevel || bChangingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->Update(dt);
  }

  // Rigidbody updatefromtransform
  for (RigidbodyComponent* component : rigidBodyComponents)
  {
    component->UpdateFromTransform();
    if (component->GetRigidBody()->isKinematic())
      component->UpdateKinematicTransform();
  }

  // phjix simulate
  if (INPUT.IsKeyPress(MouseState::LB))
    _phyjixWorld->LeftClick();
  if (INPUT.IsKeyPress(MouseState::RB))
    _phyjixWorld->RightClick();
  _phyjixWorld->UpdateRay(
      mainCamera->GetPosition(),
      Vector2(INPUT.GetCurrMouseState().x, INPUT.GetCurrMouseState().y),
      mainCamera->GetViewTransform(), mainCamera->GetProjectionMatrix(),
      Vector2(kScreenWidth, kScreenHeight));
  _phyjixWorld->CastRay();
  _phyjixEngine->Update(dt);

  // Rigidbody updateTotransform
  for (RigidbodyComponent* component : rigidBodyComponents)
  {
    if (!(component->GetOwner()->status == EStatus_Active))
      continue;

    component->UpdateToTransform();
  }

// UI Update
#ifdef USED2D
  _canvas->Update(dt);
#endif // USED2D
}

void World::AnimationUpdate(float dt)
{
  if (!_currentLevel || bChangingLevel)
    return;

  // Animation update.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    if (auto* animComp = gameObject->GetComponent<AnimatorComponent>();
        animComp)
    {
      animComp->UpdateAnimation(dt);
    }
  }

  // Update local transforms
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    auto* transform = gameObject->transform;
    if (transform->bNeedUpdateTransform)
    {
      transform->UpdateLocalTransform();
    }
  }

  // Update hierarchical transforms
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    auto* transform = gameObject->transform;
    if (transform->bNeedUpdateGlobalTransform)
    {
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        auto* transform = object->transform;
        transform->UpdateGlobalTransform();
      });
    }
  }

  // Update skeletal mesh vertex transforms
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    if (auto* skeletalMesh = gameObject->GetComponent<SkeletalMeshComponent>();
        skeletalMesh)
    {
      skeletalMesh->UpdateBoneTransforms();
    }
  }
}

void World::PostUpdate(float dt)
{
  if (!_currentLevel || bChangingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->PostUpdate(dt);
  }
}

static DirectionalLight _mainLight;
void World::RenderGameObjects()
{
  if (!_currentLevel || bChangingLevel)
    return;

  Matrix view = mainCamera->GetViewTransform();
  Matrix projection = mainCamera->GetProjectionMatrix();
  _renderer->BeginFrame(mainCamera->GetPosition(), view, projection,
                        _mainLight);
#ifdef _DEBUG
  if (ImGui::Begin("main Light"))
  {
    float _mainLightDir[3] = {_mainLight.direction.x, _mainLight.direction.y,
                              _mainLight.direction.z};
    ImGui::SliderFloat3("direction", _mainLightDir, -1.f, 1.f);
    _mainLight.direction.x = _mainLightDir[0];
    _mainLight.direction.y = _mainLightDir[1];
    _mainLight.direction.z = _mainLightDir[2];
    float _mainLightColor[4] = {_mainLight.radiance.x, _mainLight.radiance.y,
                                _mainLight.radiance.z, 1.f};
    ImGui::ColorEdit3("Color", _mainLightColor);
    float _mainLightIntencity = _mainLight.radiance.w;
    ImGui::SliderFloat("intencity", &_mainLightIntencity, 0.f, 1.f);
    _mainLight.radiance.x = _mainLightColor[0];
    _mainLight.radiance.y = _mainLightColor[1];
    _mainLight.radiance.z = _mainLightColor[2];
    _mainLight.radiance.w = _mainLightIntencity;
  }
  ImGui::End();
#endif

  // Rendering stage
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active) &&
        !(gameObject->status == EStatus_Inactive))
      continue;

    gameObject->OnRender();

    // Draw static mesh
    if (auto* meshComp = gameObject->GetComponent<MeshComponent>();
        meshComp && meshComp->isVisible)
    {
      const auto& transform = gameObject->GetWorldTransform();
      for (auto handle : meshComp->GetSubMeshes())
      {
        // _renderer->BeginDraw(handle, transform);
        _renderer->DrawMesh(handle, transform, meshComp->renderTypeFlags,
                            meshComp->outlineColor);
      }
    }
    // Draw skeletal mesh
    if (auto* skeletalMeshComp =
            gameObject->GetComponent<SkeletalMeshComponent>();
        skeletalMeshComp && skeletalMeshComp->isVisible)
    {
      auto handle = skeletalMeshComp->GetHandle();
      // const auto& transform =
      // skeletalMeshComp->rootBone->GetGlobalTransform();
      // _renderer->BeginDraw(handle, transform);
      // outline test
      _renderer->DrawMesh(
          handle, XMMatrixIdentity(), skeletalMeshComp->renderTypeFlags,
          skeletalMeshComp->outlineColor, skeletalMeshComp->boneTransforms);
    }
    // Draw Billboard
    if (auto* billboardComp = gameObject->GetComponent<BillboardComponent>();
        billboardComp && billboardComp->isVisible)
    {
      _renderer->DrawBillBoard(billboardComp->billboard);
    }
    // draw Trail
    if (auto* trailComp = gameObject->GetComponent<TrailComponent>();
        trailComp && trailComp->isVisible)
    {
      _renderer->DrawTrail(trailComp->trail);
    }
  }
#ifdef _DEBUG

  //_renderer->DrawDebugCylinder(XMMatrixScalingFromVector({0.001f, 20, 20}) *
  //                                 XMMatrixRotationAxis({0,0,1},XM_PIDIV2),
  //                        Color(1, 0, 1));
  for (auto object : _currentLevel->GetGameObjectList())
  {
    if (auto rigidbody = object->GetComponent<RigidbodyComponent>())
    {
      auto transform = object->GetComponent<TransformComponent>();
      if (rigidbody->GetDebugDrawFlag())
      {
        rigidbody->UpdateDebugDrawMatrix();
        XMMATRIX offsetMat = rigidbody->debugDrawMatrix;
        switch (rigidbody->GetRigidBody()->GetColliderShapeType())
        {
        case ColliderShape::eCubeCollider:
          _renderer->DrawDebugBox(offsetMat, rigidbody->debugColor);
          break;
        case ColliderShape::eSphereCollider:
          _renderer->DrawDebugSphere(offsetMat, rigidbody->debugColor);
          break;
        default:
          break;
        }
      }
    }
  }
#endif
}

void World::RenderUI()
{
#ifdef USED2D
  _canvas->Render();
#endif // USED2D

  _renderer->EndFrame();
}

void World::CleanupStage()
{
  if (!_currentLevel || bChangingLevel)
    return;

  // Find cleaned up game objects.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Cleanup)
    {
      // Destroy itself.
      gameObject->FinishDestroy();
    }
  }

  // Deactivation & cleanup components
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Active && gameObject->bShouldDeactivate)
    {
      // Deactivate game object hierarchy
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        object->status = EStatus_Inactive;
        object->isActive = false;
        object->bShouldDeactivate = false;
      });
    }

    // Begin destroy
    if (gameObject->bShouldDestroy)
    {
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        if (object->tag == "weapon")
        {
          int n = 0;
        }

        object->BeginDestroy();
        object->isActive = false;
        object->bShouldDestroy = false;

        // Remove hierarchical relationships
        if (object->parent)
          object->parent->RemoveChildGameObject(object);
        // Remove it from the search registrations
        object->SetName("");
        object->SetGameObjectTag("");
        object->RemoveFromTypeRegistration();
      });
    }
  }

  // Remove destroyed game objects
  for (auto it = _currentLevel->GetGameObjectList().begin();
       it != _currentLevel->GetGameObjectList().end();)
  {
    GameObject* gameObject = *it;
    if (gameObject->status == EStatus_Destroyed)
    {
      it = _currentLevel->gameObjects.erase(it);
    }
    else
    {
      ++it;
    }
  }

  // Mark that it's ready to change level.
  bReadyToChangeLevel = true;
}

void World::UpdateGameObjectHierarchy(GameObject* gameObject,
                                      std::function<void(GameObject*)> func)
{
  std::stack<GameObject*> st;

  st.push(gameObject);

  while (!st.empty())
  {
    GameObject* currObject = st.top();
    st.pop();

    func(currObject);

    for (auto* child : currObject->childrens)
    {
      st.push(child);
    }
  }
}
