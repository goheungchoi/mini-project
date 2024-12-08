#pragma once

#include <nvtt/nvtt.h>

#include <string>
#include <vector>

enum class ColorSpace
{
	kSRGB,
	kLinear
};

struct ImageData
{
  bool isNormalMap{false};
  bool isCubeMap{false}; // TODO: Support cube map

  ColorSpace colorSpace;
  nvtt::PixelType valueType;
  nvtt::AlphaMode alphaMode;

  // Supported only when isCubeMap is true
  nvtt::CubeLayout cubeLayout;
};

struct CompressionOptions
{
  // TODO: Adjust format based on the channel.
  // Or, recommended settings.
  nvtt::Format format;
  nvtt::Quality quality;

  // Mipmap settings
  bool enableGammaCorrect;
  bool enablePremultipliedAlphaBlending;
  bool enableMipmap;
  int maxMipmapCount; // if the value < 0 or > max, set to max
  size_t minMipmapSize;
  nvtt::MipmapFilter mipmapFilter;

  // Use GPU
  bool useGPU{true};
};


bool compress(const std::string& texturePath, const std::string& exportPath,
              const ImageData& data, const CompressionOptions& options);
