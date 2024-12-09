#pragma once

#include "Core/Common.h"

#include "dxgiformat.h"

enum class TextureType : uint8_t
{
  UNKNOWN = 0x00,
  DIFFUSE,
  SPECULAR,
  AMBIENT,
  EMISSIVE,
  HEIGHT,
  SHININESS,
  OPACITY,

  ALBEDO,
  NORMAL,
  METALIC,
  ROUGHNESS,
  METALIC_ROUGHNESS,
  AMBIENT_OCCLUSION,

  INVALID_TYPE = 0xFF
};

struct TextureData {
  TextureType type;
	
	bool isSRGB;
  bool isCubeMap;

  DXGI_FORMAT format;

	uint32_t width;
  uint32_t height;

	uint32_t mipLevels;
  uint32_t arrayLayers;	// Cube map has 6 layers

	std::vector<uint8_t> data;
};
