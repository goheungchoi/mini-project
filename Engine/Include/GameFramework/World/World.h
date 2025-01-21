#pragma once

#include "GameFramework/Level/Level.h"

class World
{
protected:
  std::map<std::string, Level*> levelList;
  Level* nextLevel{nullptr};
  Level* currentLevel{nullptr};

  class CameraComponent* mainCamera{nullptr};

  // UI System
  // class Canvas* canvas = nullptr;

  // EffectSystem
  // EffectSystem effectSystem;

  // Collision System
  // CollisionSystem collisionSystem;

public:
  World() {}
  virtual ~World() {}

	template <LevelType T>
  T* CreateLevel(const std::string& name)
  {
    T* pLevel = new T(this, name);
    levelList.insert({name, pLevel});
    return pLevel;
  }

	void ChangeScene() { /* TODO: */ }
  void SetNextScene(std::string next) { /* TODO: */ }
  Level* GetCurrentLevel() { return currentLevel; }

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

};
