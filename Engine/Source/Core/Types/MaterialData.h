#pragma once

#include "Core/Common.h"

#include "Core/UUID/UUID.h"

enum class MaterialPassType {
	kUnknown,
	kOpacity,
	kTransparent,
	kLight,
	kShadow
};

struct MaterialData {
  MaterialPassType passType;
  std::string name;



};
