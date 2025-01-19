#pragma once

#include "Core/Common.h"

#include "Core/Math/MathUtils.h"

using BoneId = int;
using BoneWeight = float;

struct Bone
{
  BoneId id;
  std::string name;
  XMMATRIX inverseBindMatrix;
};
