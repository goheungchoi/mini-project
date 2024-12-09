#include "TextureExport.h"

#include "stb_image.h"

#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

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

bool compress(const std::string& texturePath, const std::string& exportPath,
              const ImageData& data, const CompressionOptions& options)
{
  bool dds10{true};

  bool SNorm{false};

  if (!fs::exists(fs::path(texturePath)) ||
      fs::is_directory(fs::path(texturePath)))
  {
    return false;
  }

  // Conpression option
  nvtt::CompressionOptions compressionOptions;
  compressionOptions.setFormat(options.format);

  if (options.format == nvtt::Format_BC2)
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
  else if (options.format == nvtt::Format_BC1a)
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
  else if (options.format == nvtt::Format_BC4S ||
           options.format == nvtt::Format_BC5S)
  {
    SNorm = true;
    compressionOptions.setPixelType(nvtt::PixelType_SignedNorm);
  }
  else if (options.format == nvtt::Format_BC6U)
  {
    compressionOptions.setPixelType(nvtt::PixelType_UnsignedFloat);
  }
  else if (options.format == nvtt::Format_BC6S)
  {
    compressionOptions.setPixelType(nvtt::PixelType_Float);
  }
  else if (options.format == nvtt::Format_RGBA)
  {
		
	}

  if (data.alphaMode != nvtt::AlphaMode_None && options.enableAlphaDithering &&
      options.format != nvtt::Format_BC2 && options.format != nvtt::Format_BC1a)
  {
    unsigned char bits = 8;
    compressionOptions.setPixelFormat(8, 8, 8, bits);
    compressionOptions.setQuantization(/*color dithering*/ false,
                                       /*alpha dithering*/ true,
                                       /*binary alpha*/ false);
  }

  compressionOptions.setQuality(options.quality);

  // Automatically use dds10 if compressing to BC6 or BC7
  if (options.format == nvtt::Format_BC6U ||
      options.format == nvtt::Format_BC6S || options.format == nvtt::Format_BC7)
  {
    dds10 = true;
  }

  nvtt::Context context(options.useGPU);

  std::vector<char> input = read_file(texturePath);
  if (input.empty())
  {
    return false;
  }

  const bool isInputDDS =
      (input.size() >= 4 && input[0] == 'D' && input[1] == 'D' &&
       input[2] == 'S' && input[3] == ' ');

  nvtt::Surface image;
  nvtt::SurfaceSet images;

  nvtt::TextureType textureType{nvtt::TextureType_2D};

  if ((options.maxMipmapCount > 1) && isInputDDS)
  {
    if (images.loadDDSFromMemory(input.data(), input.size()))
    {
      textureType = images.GetTextureType();

      image = images.GetSurface(0, 0, SNorm);
    }
  }

  if (image.isNull())
  {
    if (!image.loadFromMemory(input.data(), input.size(), nullptr, SNorm))
    {
      printf("Can't open the input file '%s'", texturePath);
      return false;
    }
    textureType = image.type();
  }

  nvtt::AlphaMode alphaMode = image.alphaMode();
  if (options.format == nvtt::Format_BC6U ||
      options.format == nvtt::Format_BC6S)
    alphaMode = nvtt::AlphaMode_None;
	


  return false;
}
