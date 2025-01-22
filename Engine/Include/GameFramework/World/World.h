#pragma once

#include "Core/Common.h"

#include "GameFramework/Level/Level.h"

#include "Renderer/IRenderer.h"

class World
{
private:

  IRenderer* _renderer;

  // UI System
  // class Canvas* _canvas = nullptr;

  // EffectSystem
  // EffectSystem _effectSystem;

  // Collision System
  // CollisionSystem _collisionSystem;

  class Camera* _defaultCamera;

public:
  std::map<std::string, Level*> _levelMap;
  Level* _nextLevel{nullptr};
  Level* _currentLevel{nullptr};

  bool bCameraMove{true};
  class Camera* mainCamera{nullptr};

  World() = default;
  virtual ~World() = default;

  void Initialize(HWND hwnd, const std::wstring& title);
  void Cleanup();

  static World* CreateWorld(HWND hwnd, const std::wstring& title);


	template <LevelType T>
  T* CreateLevel(const std::string& name)
  {
    T* pLevel = new T(this, name);
    _levelMap.insert({name, pLevel});
    return pLevel;
  }

	void ChangeLevel() { /* TODO: */ }
  void AddLevel(const std::string& level) { /* TODO: */ }
  Level* GetCurrentLevel() { return _currentLevel; }

  void SetMainCamera(class Camera* camera);
  void FixCameraPosition(bool fixed);

  XMVECTOR ScreenToWorldPosition(XMVECTOR position);

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

  void BeginPlay();
  void EndPlay();

  void ProcessInput(float dt);
  void FixedUpdate(float dt);
  void PreUpdate(float dt);
  void Update(float dt);
  void PostUpdate(float dt);

  void Render();

};
