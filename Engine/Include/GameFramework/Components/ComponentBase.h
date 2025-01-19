#pragma once

#include "GameFramework/Common.h"

class ComponentBase
{
  class GameObject* _owner{nullptr};

public:

	ComponentBase(class GameObject* owner) : _owner{owner} {}
  virtual ~ComponentBase() {}

	void SetOwner(class GameObject* owner) { _owner = owner; }
  class GameObject* GetOwner() const { return _owner; }
  class World* GetWorld() const;
};

template<class T>
concept ComponentType = std::is_base_of<ComponentBase, T>::value;
