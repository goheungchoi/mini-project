#include "TextureExport.h"

#include "stb_image.h"
#include "nv_dds.h"
#include "dxgiformat.h"

#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

DXGI_FORMAT todx(ImageFormat format, bool srgb)
{
  switch (format)
  {
  case ImageFormat::BC7:
    if (srgb)
      return DXGI_FORMAT_BC7_UNORM_SRGB;
    else
      return DXGI_FORMAT_BC7_UNORM;
  case ImageFormat::BC6S:
    return DXGI_FORMAT_BC6H_SF16;
  case ImageFormat::BC6U:
    return DXGI_FORMAT_BC6H_UF16;
  case ImageFormat::BC5u:
    return DXGI_FORMAT_BC5_UNORM;
  case ImageFormat::BC4u:
    return DXGI_FORMAT_BC4_UNORM;
  case ImageFormat::BC3:
    if (srgb)
      return DXGI_FORMAT_BC3_UNORM_SRGB;
    else
      return DXGI_FORMAT_BC3_UNORM;
  case ImageFormat::BC3n:
    return DXGI_FORMAT_BC3_UNORM;
  case ImageFormat::BC2:
    if (srgb)
      return DXGI_FORMAT_BC2_UNORM_SRGB;
    else
      return DXGI_FORMAT_BC2_UNORM;
  case ImageFormat::BC1a:
  case ImageFormat::BC1:
    if (srgb)
      return DXGI_FORMAT_BC1_UNORM_SRGB;
    else
      return DXGI_FORMAT_BC1_UNORM;
  case ImageFormat::R8_UINT:
    return DXGI_FORMAT_R8_UINT;
  case ImageFormat::R8G8_UINT:
    return DXGI_FORMAT_R8G8_UINT;
  case ImageFormat::R8G8B8A8_UINT:
    if (srgb)
      return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    else
      return DXGI_FORMAT_R8G8B8A8_UNORM;
  case ImageFormat::R16_FLOAT:
    return DXGI_FORMAT_R16_FLOAT;
  case ImageFormat::R16G16_FLOAT:

    return DXGI_FORMAT_R16G16_FLOAT;
  case ImageFormat::R16G16B16A16_FLOAT:
    return DXGI_FORMAT_R16G16B16A16_FLOAT;
  case ImageFormat::R32_FLOAT:
    return DXGI_FORMAT_R32_FLOAT;
  case ImageFormat::R32G32_FLOAT:
    return DXGI_FORMAT_R32G32_FLOAT;
  case ImageFormat::R32G32B32_FLOAT:
    return DXGI_FORMAT_R32G32B32_FLOAT;
  case ImageFormat::R32G326A32_FLOAT:
    return DXGI_FORMAT_R32G32B32A32_FLOAT;
  }
  return DXGI_FORMAT_UNKNOWN;
}

[[nodiscard]] 
nvtt::Format tonv(ImageFormat format) 
{
  switch (format)
  {
  case ImageFormat::BC7:
    return nvtt::Format_BC7;
  case ImageFormat::BC6S:
    return nvtt::Format_BC6S;
  case ImageFormat::BC6U:
    return nvtt::Format_BC6U;
  case ImageFormat::BC5u:
    return nvtt::Format_BC5;
  case ImageFormat::BC4u:
    return nvtt::Format_BC4;
  case ImageFormat::BC3:
    return nvtt::Format_BC3;
  case ImageFormat::BC3n:
    return nvtt::Format_BC3n;
  case ImageFormat::BC2:
    return nvtt::Format_BC2;
  case ImageFormat::BC1a:
    return nvtt::Format_BC1a;
  case ImageFormat::BC1:
    return nvtt::Format_BC1;
  case ImageFormat::R8_UINT:
  case ImageFormat::R8G8_UINT:
  case ImageFormat::R8G8B8A8_UINT:
  case ImageFormat::R16_FLOAT:
  case ImageFormat::R16G16_FLOAT:
  case ImageFormat::R16G16B16A16_FLOAT:
  case ImageFormat::R32_FLOAT:
  case ImageFormat::R32G32_FLOAT:
  case ImageFormat::R32G32B32_FLOAT:
  case ImageFormat::R32G326A32_FLOAT:
  case ImageFormat::kNumFormats:
    return nvtt::Format_Unset;
  }
}

[[nodiscard]] 
static std::vector<char> read_file(const fs::path& filepath)
{
  std::vector<char> result;

	try
  {
    // Open the file with the cursor at the end
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
      throw std::runtime_error("File can't be opened.");
    }

    // The location of the cursor tells the size of
    // the file in bytes
    unsigned long long filesize = static_cast<unsigned long long>(file.tellg());

    if (filesize < 0)
    {
      throw std::runtime_error("File length is negative.");
		}

		result.resize(filesize);
   
    // Place the fie cursor at the beginning
    file.seekg(0);

    // Load the entire file into the buffer
    file.read(result.data(), filesize);

    // Close the file
    file.close();
	}
  catch (const std::exception& e)
  {
    printf("%s", e.what());
    return result;
	}
  

  // Return true
  return result;
}

bool textureExport(const std::string& texturePath, const std::string& exportPath,
              const ImageData& data, const TextureExportOptions& options)
{
  if (!fs::exists(fs::path(texturePath)) ||
      fs::is_directory(fs::path(texturePath)))
  {
    return false;
  }

  auto input = read_file(fs::path(texturePath));

  const bool isInputDDS =
      (input.size() >= 4 && input[0] == 'D' && input[1] == 'D' &&
       input[2] == 'S' && input[3] == ' ');

	if (isInputDDS)
  {
    bool dds10{true};

    bool SNorm{false};

    // Conpression option
    nvtt::CompressionOptions compressionOptions;
    compressionOptions.setFormat(tonv(options.format));

    if (tonv(options.format) == nvtt::Format_BC2)
    {
      if (options.enableAlphaDithering)
      {
        unsigned char bits = 4;
        // Dither alpha when using BC2.
        compressionOptions.setPixelFormat(8, 8, 8, bits);
        compressionOptions.setQuantization(/*color dithering*/ false,
                                           /*alpha dithering*/ true,
                                           /*binary alpha*/ false);
      }
    }
    else if (tonv(options.format) == nvtt::Format_BC1a)
    {
      if (options.enableAlphaDithering)
      {
        // Binary alpha when using BC1a.
        compressionOptions.setQuantization(/*color dithering*/ false,
                                           /*alpha dithering*/ true,
                                           /*binary alpha*/ true, 127);
      }
      else
      {
        compressionOptions.setQuantization(/*color dithering*/ false,
                                           /*alpha dithering*/ false,
                                           /*binary alpha*/ true, 127);
      }
    }
    else if (tonv(options.format) == nvtt::Format_BC4S ||
             tonv(options.format) == nvtt::Format_BC5S)
    {
      SNorm = true;
      compressionOptions.setPixelType(nvtt::PixelType_SignedNorm);
    }
    else if (tonv(options.format) == nvtt::Format_BC6U)
    {
      compressionOptions.setPixelType(nvtt::PixelType_UnsignedFloat);
    }
    else if (tonv(options.format) == nvtt::Format_BC6S)
    {
      compressionOptions.setPixelType(nvtt::PixelType_Float);
    }
    else if (tonv(options.format) == nvtt::Format_RGBA)
    {
    }

    if (data.alphaMode != nvtt::AlphaMode_None &&
        options.enableAlphaDithering &&
        tonv(options.format) != nvtt::Format_BC2 &&
        tonv(options.format) != nvtt::Format_BC1a)
    {
      unsigned char bits = 8;
      compressionOptions.setPixelFormat(8, 8, 8, bits);
      compressionOptions.setQuantization(/*color dithering*/ false,
                                         /*alpha dithering*/ true,
                                         /*binary alpha*/ false);
    }

    compressionOptions.setQuality(options.quality);

    // Automatically use dds10 if compressing to BC6 or BC7
    if (tonv(options.format) == nvtt::Format_BC6U ||
        tonv(options.format) == nvtt::Format_BC6S ||
        tonv(options.format) == nvtt::Format_BC7)
    {
      dds10 = true;
    }

    nvtt::Context context(options.useGPU);

    // std::vector<char> input = read_file(texturePath);
    if (input.empty())
    {
      return false;
    }

    if (data.isCubeMap)
    {
      nvtt::CubeSurface;
    }
	}
  else
  {
		// Uncompressed normal data
    void* image;
    int width, height, channels, ok;
    // Check if the file is a valid texture
    ok = stbi_info_from_memory((uint8_t*)input.data(), input.size(), &width, &height, &channels);
    if (!ok)
    {
      return false;
    }

		const int requiredChannels = GetNumChannels(options.format);

		const bool hasAlpha = (channels == 4);
		const bool isHdr =
        stbi_is_hdr_from_memory((uint8_t*)input.data(), input.size());
    const bool is16bits =
        stbi_is_16_bit_from_memory((uint8_t*)input.data(), input.size());

		nvtt::InputFormat inputFormat;
		
		if (IsUncompressedFormat(options.format))
    {
      size_t dataByteSize{0};
      if (IsHDR(options.format))
      {
        if (Is16Bits(options.format))
        {
          image = stbi_load_16_from_memory((uint8_t*)input.data(), input.size(),
                                           &width, &height, &channels, requiredChannels);
          dataByteSize = width * height * requiredChannels * 2;
        }
        else
        {
          image = stbi_loadf_from_memory((uint8_t*)input.data(), input.size(),
                                         &width, &height, &channels,
                                         requiredChannels);
          dataByteSize = width * height * requiredChannels * 4;
				}
      }
      else
      {
        image =
            stbi_load_from_memory((uint8_t*)input.data(), input.size(), &width,
                                  &height, &channels, requiredChannels);
        dataByteSize = width * height * requiredChannels;
			}

			nv_dds::Image dds;
      dds.allocate();
      dds.subresource().create(dataByteSize, image);
      dds.mip0Width = width;
      dds.mip0Height = height;
      dds.mip0Depth = 1;
      dds.dxgiFormat = todx(options.format, data.colorSpace == ColorSpace::kSRGB);
      dds.alphaMode = data.alphaMode == nvtt::AlphaMode_None
                          ? nv_dds::DDS_ALPHA_MODE_OPAQUE
                          : nv_dds::DDS_ALPHA_MODE_STRAIGHT;
      dds.isNormal = data.isNormalMap;

			dds.writeToFile(exportPath.c_str(), {.useDx10HeaderIfPossible = true});

			stbi_image_free(image);
			return true;
		}
    else
    {
      /*if (isHdr)
    {
      if (is16bits)
      {
        image = stbi_load_16_from_memory((uint8_t*)input.data(), input.size(),
                                         &width, &height, &channels,
                                         4);
        inputFormat = nvtt::InputFormat_RGBA_16F;
      } else {
        image =
            stbi_loadf_from_memory((uint8_t*)input.data(), input.size(), &width,
                                   &height, &channels, 4);
        inputFormat = nvtt::InputFormat_RGBA_32F;
      }
    }
    else
    {
      image =
          stbi_load_from_memory((uint8_t*)input.data(), input.size(), &width,
                                &height, &channels, 4);
      inputFormat = nvtt::InputFormat_BGRA_8UB;
    }

    if (!image)
    {
      return false;
    }*/
		}
	}


  /*if (image.isNull())
  {
    if (!image.loadFromMemory(input.data(), input.size(), nullptr, SNorm))
    {
      printf("Can't open the input file '%s'", texturePath);
      return false;
    }
    textureType = image.type();
  }*/

  /*nvtt::AlphaMode alphaMode = image.alphaMode();
  if (options.format == nvtt::Format_BC6U ||
      options.format == nvtt::Format_BC6S)
    alphaMode = nvtt::AlphaMode_None;*/
	


  return false;
}
