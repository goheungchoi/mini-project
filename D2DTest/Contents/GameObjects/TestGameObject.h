#pragma once

#include "GameFramework/GameObject/GameObject.h"

class TestGameObject : public GameObject
{
public:
	TestGameObject(World* world);

public:
	void OnRender() override;
};


