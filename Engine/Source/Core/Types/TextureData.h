#pragma once

#include "Core/Common.h"

#include "dxgiformat.h"

enum class TextureType : uint8_t
{
  kUnknown = 0x00,
  kDiffuse,
  kSpecular,
  kAmbient,
	kEmissive,
  kHeight,
  kShininess,
  kOpacity,
  kDisplacement,

  kAlbedo,
	kNormal,
  kMetalic,
  kRoughness,
  kMetalicRoughness,
  kAmbientOcclusion,

  kInvalidType = 0xFF
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

	std::vector<uint8_t> ddsData;
};
