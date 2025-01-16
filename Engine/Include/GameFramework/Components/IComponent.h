#pragma once

#include "GameFramework/Common.h"

class IComponent
{
  class Actor* _owner{nullptr};

public:

	IComponent(Actor* owner) : _owner{owner} {}
  virtual ~IComponent() {}

	void SetOwner(class Actor* owner) { _owner = owner; }
  class Actor* GetOwner() const { return _owner; }
  class World* GetWorld() const;
};

template<class T>
concept ComponentType = std::is_base_of<IComponent, T>::value;
