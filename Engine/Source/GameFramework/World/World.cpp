#include "GameFramework/World/World.h"

#include "Renderer/DX11/DX11Renderer.h"

#include "Core/Input/InputSystem.h"
#include "Core/Camera/Camera.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"

#include "Interface.h"
#include "PhyjixEngine.h"

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif

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
  _phyjixWorld->CreateRay(
      _defaultCamera->GetPosition(),
      Vector2(Input.GetCurrMouseState().x, Input.GetCurrMouseState().y),
      _defaultCamera->GetViewTransform(), 
      _defaultCamera->GetProjectionMatrix(),
      Vector2(kScreenWidth, kScreenHeight));

  SetMainCamera(_defaultCamera);
}

World* World::CreateWorld(HWND hwnd, const std::wstring& title)
{
  World* newWorld = new World();
  newWorld->Initialize(hwnd, title);

  return newWorld;
}

void World::PrepareChangeLevel(const std::string& levelName) {
  /* TODO: */
  if (auto it = _levelMap.find(levelName); it != _levelMap.end())
  {
    _preparingLevel = it->second;
    
    _preparingLevel->PrepareLevel();
  }
  else
  {
    throw std::runtime_error("Level is not found: " + levelName);
  }
}

bool World::IsLevelChangeReady() {
  if (!_preparingLevel)
    return false;

  /* TODO: */
  auto& f = _levelPreparedMap[_preparingLevel->name];

  return f.valid() && f.wait_for(0s) == std::future_status::ready && f.get();
}

void World::CommitLevelChange() {
  //async::executor.async([&]() {
  //  auto& f = _levelPreparedMap[_preparingLevel->name];
  //  f.get();

  //  while (!readyToChangeLevel) {}  // Wait until
    changingLevel = true;
    
    if (_currentLevel)
    {
      _currentLevel->DestroyLevel();
      _currentLevel->CleanupLevel();
    }
    

    _currentLevel = _preparingLevel;
    _currentLevel->BeginLevel();

    changingLevel = false;
  // });
}

void World::AddLevel(Level* level) {
  level->world = this;
  _levelMap[level->name] = level;
}

void World::SetMainCamera(Camera* camera) {
  if (camera)
    mainCamera = camera;
  else
    mainCamera = _defaultCamera;
}

void World::StopCameraMovement(bool fixed)
{
  bCameraMove = !fixed;
}

XMVECTOR World::ScreenToWorldPosition(XMVECTOR screenPos)
{


  return XMVECTOR();
}

void World::RegisterGameObjectName(GameObject* gameObject) {
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

void World::UnregisterGameObjectType(GameObject* gameObject) {
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

void World::RegisterMeshComponent(SkeletalMeshComponent* skeletalMeshComp) {
  if (!skeletalMeshComp)
    return;

  _renderer->CreateMesh(skeletalMeshComp->GetHandle());
  if (skeletalMeshComp->bCastShadow)
    _renderer->AddShadow(skeletalMeshComp->GetHandle());
}

void World::InitialStage() {
  if (!_currentLevel || changingLevel)
    return;

  // Mark that it should not change the level while running the game loop.
  readyToChangeLevel = false;

  // Awake and activate game objects.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Awake && gameObject->bShouldActivate)
    {
      // Awake the game objects and activate it.
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        object->OnAwake();
        object->OnActivated();
        object->isActive = true;
        object->status = EStatus_Active;
        object->bShouldActivate = false;
      });
    } 

    if (gameObject->status == EStatus_Inactive && gameObject->bShouldActivate)
    {
      UpdateGameObjectHierarchy(gameObject, [](GameObject* object) {
        // Activate the game object.
        object->OnActivated();
        object->isActive = true;
        object->status = EStatus_Active;
        object->bShouldActivate = false;
      });
    }
  }
}

void World::FixedUpdate(float fixedRate)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->FixedUpdate(fixedRate);
  }
}

void World::PhysicsUpdate(float fixedRate) {
  if (!_currentLevel || changingLevel)
    return;

  // TODO:
}

void World::ProcessInput(float dt)
{
  InputSystem::GetInstance()->Update(dt);

  // _camera->LookAt({10, 0, 0, 1});
  // 'Q' 누르면 Down
  if (Input.IsKeyPress(Key::Q))
  {
    mainCamera->MoveDownUp(-dt);
  }
  // 'E' 누르면 Up
  if (Input.IsKeyPress(Key::E))
  {
    mainCamera->MoveDownUp(dt);
  }
  // 'A' 누르면 Left
  if (Input.IsKeyPress(Key::A))
  {
    mainCamera->MoveLeftRight(-dt);
  }
  // 'D' 누르면 Right
  if (Input.IsKeyPress(Key::D))
  {
    mainCamera->MoveLeftRight(dt);
  }
  // 'W' 누르면 Forward
  if (Input.IsKeyPress(Key::W))
  {
    mainCamera->MoveBackForward(dt);
  }
  // 'S' 누르면 Backward
  if (Input.IsKeyPress(Key::S))
  {
    mainCamera->MoveBackForward(-dt);
  }
  if (Input.IsKeyPress(Key::OemMinus))
  {
    mainCamera->AddMoveSpeed(-10);
  }
  if (Input.IsKeyPress(Key::OemPlus))
  {
    mainCamera->AddMoveSpeed(+10);
  }

  if (Input.IsKeyDown(MouseState::LB))
  {
    _phyjixWorld->LeftClick();

  }
  if (Input.IsKeyDown(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!bCameraMove);   // 커서가 안 보임
  }
  if (Input.IsKeyUp(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!bCameraMove);   // 커서가 보임
  }

  // 마우스 회전
  if (bCameraMove)
  {
    // 마우스 이동량 가져오기
    Vector2 mouseDelta = Input.GetMouseDelta();
    float x = -mouseDelta.x;
    float y = -mouseDelta.y;

    /*if ((Input.GetCurrMouseState().x != Input.GetPrevMouseState().x) ||
        (Input.GetCurrMouseState().y != Input.GetPrevMouseState().y))
    {*/
      // 마우스가 Y 축으로 움직이면 X 축 회전
      mainCamera->RotateAroundXAxis(y);
      // 마우스가 X 축으로 움직이면 Y 축 회전
      mainCamera->RotateAroundYAxis(x);
    //}
  }
}

void World::PreUpdate(float dt) {
  if (!_currentLevel || changingLevel)
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
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->Update(dt);
  }

  ////Rigidbody updatefromtransform
  //for (RigidbodyComponent* component : _currentLevel->GetRigidbodyList())
  //{
  //  component->UpdateFromTransform();
  //}


  //phjix simulate
  _phyjixWorld->UpdateRay(
      _defaultCamera->GetPosition(),
      Vector2(Input.GetCurrMouseState().x, Input.GetCurrMouseState().y),
      _defaultCamera->GetViewTransform(), _defaultCamera->GetProjectionMatrix(),
      Vector2(kScreenWidth, kScreenHeight));
  _phyjixWorld->CastRay();
  _phyjixEngine->Update(dt);

    // Rigidbody updateTotransform
  for (RigidbodyComponent* component : _currentLevel->GetRigidbodyList())
  {
    component->UpdateToTransform();
  }


}

void World::AnimationUpdate(float dt)
{
  if (!_currentLevel || changingLevel)
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
}

void World::PostUpdate(float dt)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!(gameObject->status == EStatus_Active))
      continue;

    gameObject->PostUpdate(dt);
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

static DirectionalLight _mainLight;
void World::RenderGameObjects() {
  if (!_currentLevel || changingLevel)
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
    _mainLight.radiance = {1.f, 1.f, 1.f, 1.f};
  }
  ImGui::End();
  #endif

  // Rendering stage
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    gameObject->OnRender();

    // Draw static mesh
    if (auto* meshComp = gameObject->GetComponent<MeshComponent>();
        meshComp)
    {
      const auto& transform = gameObject->GetWorldTransform();
      for (auto handle : meshComp->GetSubMeshes())
      {
        // NOTE : ADD draw mesh -> flag
        RenderTypeFlags flag = 0;
        // _renderer->BeginDraw(handle, transform);
        _renderer->DrawMesh(handle, transform,flag);
			}
    }
    // Draw skeletal mesh
    else if (auto* skeletalMeshComp = gameObject->GetComponent<SkeletalMeshComponent>();
             skeletalMeshComp)
    {
      auto handle = skeletalMeshComp->GetHandle();
      // const auto& transform = skeletalMeshComp->rootBone->GetGlobalTransform();
      // _renderer->BeginDraw(handle, transform);
      // outline test 
      // NOTE : ADD draw mesh -> flag
      RenderTypeFlags flag = 0;
      flag |= RenderType::kOutline;
      _renderer->DrawMesh(handle, XMMatrixIdentity(),flag,
                          skeletalMeshComp->boneTransforms);
    }
  }
#ifdef _DEBUG
  for (auto object : _currentLevel->GetGameObjectList())
  {
    if (auto rigidbody = object->GetComponent<RigidbodyComponent>())
    {
      auto transform = object->GetComponent<TransformComponent>();
      if (rigidbody->GetDebugDrawFlag())
      {
        switch (rigidbody->GetRigidBody()->GetColliderShapeType())
        {
        case ColliderShape::eCubeCollider:
          _renderer->DrawDebugBox(transform->GetGlobalTransform(),
                                  Color(1, 0, 1));
          break;
        case ColliderShape::eSphereCollider:
          _renderer->DrawDebugSphere(transform->GetGlobalTransform(),
                                     Color(1, 0, 1));
          break;
        default:
          break;

        }
      }
    }
  }
#endif



  _renderer->EndFrame();
}

void World::RenderUI() {
  // TODO:

}

void World::CleanupStage() {
  if (!_currentLevel || changingLevel)
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
        object->BeginDestroy();
        object->isActive = false;
        object->bShouldDestroy = false;

				// Remove hierarchical relationships
        object->parent->RemoveChild(object);
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
  readyToChangeLevel = true;
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


