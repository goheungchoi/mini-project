#pragma once

#include "Core/Common.h"

#include "Core/Handle/ResourceHandle.h"

#include "BoneData.h"

/**
 * @brief The hierarchy data of the skeleton node.
 */
struct SkeletonNode
{
  std::string name;
  XMMATRIX transform;

  int level;
  int parent;
  int firstChild;
  int nextSibling;

	BoneId boneId;
};

/**
 * @brief The whole skeleton structure data in a model.
 * This is used to bind skeletal animation data and skinned meshes.
 */
struct SkeletonData
{
  std::string name;

	std::vector<Bone> bones;
  std::vector<SkeletonNode> nodes;
};
