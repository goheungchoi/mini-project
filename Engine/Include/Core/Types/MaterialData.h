#pragma once

#include "Core/Common.h"

#include "Core/UUID/UUID.h"
#include "Core/Math/MathUtils.h"

#include "Core/Handle/ResourceHandle.h"

enum class AlphaMode
{
	kOpaque,	// Alpha value is ignored
	kMask,		// Alpha cutoff
	kBlend		// Blended with the background
};

struct MaterialData {
  // MaterialPassType passType;
  std::string name;

	Color albedoFactor;
  TextureHandle albedoTexture;

	float metallicFactor;
  float roughnessFactor;
  TextureHandle metallicRoughnessTexture;

	TextureHandle normalTexture;

	TextureHandle occlusionTexture;

	float emissiveFactor;
  TextureHandle emissiveTexture;

	AlphaMode alphaMode;
  float alphaCutoff;
  bool doubleSided;
};
