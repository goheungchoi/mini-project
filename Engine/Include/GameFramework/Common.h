#pragma once

#include "Core/Common.h"

#include "ResourceManager/ResourceManager.h"

enum EStatus
{
  EStatus_Awake,
  EStatus_Inactive,
  EStatus_Active,
  EStatus_Cleanup,
  EStatus_Destroyed
};

