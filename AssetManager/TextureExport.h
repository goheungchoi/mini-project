#pragma once

#include "nvtt/nvtt.h"

#include <string>
#include <vector>

#include "ImageFormatEnum.h"

enum class ColorSpace
{
	kSRGB,
	kLinear
};

struct ImageData
{
  bool isNormalMap;
  bool isCubeMap;

  ColorSpace colorSpace;
  nvtt::AlphaMode alphaMode;
};

struct TextureExportOptions
{
  // TODO: Adjust format based on the channel.
  // Or, recommended settings.
  ImageFormat format;
  nvtt::Quality quality;

  // Mipmap settings
  bool enableGammaCorrect;
  bool enablePremultipliedAlphaBlending;
  bool enableAlphaDithering;
  bool enableMipmap;
  int maxMipmapCount; // if the value =< 0 or > max, set to max
  size_t minMipmapSize;
  nvtt::MipmapFilter mipmapFilter;

	// Supported only when isCubeMap is false
  bool convertToCubeMap;
  nvtt::CubeLayout cubeLayout;

  // Use GPU
  bool useGPU{true};
};


bool exportTextureFromMemory(const std::vector<char>& input,
                             const std::string& exportPath,
                             const ImageData& data,
                             const TextureExportOptions& options);

bool exportTextureFromFile(const std::string& texturePath, const std::string& exportPath,
              const ImageData& data, const TextureExportOptions& options);

