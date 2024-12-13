#include "TextureExport.h"

#include <Windows.h>

#include "stb_image.h"
#define NV_DDS_UTILITY_VALUES
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

// Attempts to convert an fs::path to a string in the app's code page (ANSI on
// Windows, UTF-8 on other systems). Replaces unprintable characters with
// the system default character. As a last resort, returns an empty string.
std::string toPrintableString(const fs::path& path)
{
  try
  {
#ifdef _WIN32
    // We avoid using <codecvt> here because <codecvt> is deprecated:
    static_assert(std::is_same_v<fs::path::string_type, std::wstring>);
    const std::wstring& str = path.native();

    // Get the size of the output:
    const int ansiCharacters = WideCharToMultiByte(
        CP_ACP, /* CodePage */
        WC_COMPOSITECHECK | WC_DEFAULTCHAR | WC_NO_BEST_FIT_CHARS, /* dwFlags */
        str.c_str(), /* lpWideCharStr */
        -1,          /* cchWideChar */
        nullptr,     /* lpMultiByteStr */
        0,           /* cbMultiByte */
        nullptr,     /* lpDefaultChar */
        nullptr);    /* lpUsedDefaultChar */
    if (ansiCharacters < 0)
    {
      fprintf(stderr, "toPrintableString(): WideCharToMultiByte() failed.\n");
      return {};
    }
    // Allocate space:
    std::string result(static_cast<size_t>(ansiCharacters), 0);
    // Perform the conversion:
    std::ignore = WideCharToMultiByte(CP_ACP, /* CodePage */
                                      WC_COMPOSITECHECK | WC_DEFAULTCHAR |
                                          WC_NO_BEST_FIT_CHARS, /* dwFlags */
                                      str.c_str(),    /* lpWideCharStr */
                                      -1,             /* cchWideChar */
                                      result.data(),  /* lpMultiByteStr */
                                      ansiCharacters, /* cbMultiByte */
                                      nullptr,        /* lpDefaultChar */
                                      nullptr);       /* lpUsedDefaultChar */
    return result;
#else
    // Linux uses UTF-8 paths and can print UTF-8, so this is easier:
    return path.string();
#endif
  }
  catch (const std::exception& e)
  {
    fprintf(stderr, "toPrintableString() threw an unexpected exception: %s\n",
            e.what());
    return {};
  }
}

// An OutputHandler that uses an std::filesystem::path.
// This allows us to write to Windows paths that use characters outside of the
// ANSI character set. (Windows allows UTF-16 plus unpaired surrogates.)
struct PathOutputHandler : public nvtt::OutputHandler
{
private:
  std::ofstream m_file;
  fs::path m_path;

public:
  explicit PathOutputHandler(const fs::path& path) : m_path(path)
  {
    try
    {
      m_file = std::ofstream(path, std::ios::binary | std::ios::out);
    }
    catch (const std::exception& e)
    {
      fprintf(stderr, "Could not open %s: %s\n",
              toPrintableString(path).c_str(), e.what());
    }
  }

  void beginImage(int size, int width, int height, int depth, int face,
                  int mipLevel) override
  {
  }

  bool writeData(const void* data, int size) override
  {
    if (size < 0)
      return false;
    try
    {
      m_file.write(reinterpret_cast<const char*>(data),
                   static_cast<std::streamsize>(size));
    }
    catch (const std::exception& e)
    {
      fprintf(stderr, "Could not write to %s: %s\n",
              toPrintableString(m_path).c_str(), e.what());
      return false;
    }
    return true;
  }

  void endImage() override {}
};

bool textureExport(const std::string& texturePath, const std::string& exportPath,
              const ImageData& data, const TextureExportOptions& options)
{
  if (!fs::exists(fs::path(texturePath)) ||
      fs::is_directory(fs::path(texturePath)))
  {
    return false;
  }

  auto input = read_file(fs::path(texturePath));
	if (input.empty())
  {
    return false;
	}

  const bool isInputDDS =
      (input.size() >= 4 && input[0] == 'D' && input[1] == 'D' &&
       input[2] == 'S' && input[3] == ' ');

	// DDS
	if (isInputDDS)
  {
    const bool alpha = (data.alphaMode != nvtt::AlphaMode_None);
    const bool normal = data.isNormalMap;
		const nvtt::Format nvttFormat = tonv(options.format);
    const bool bc1n = (options.format == ImageFormat::BC1) && normal;

    bool dds10{true};

    // Conpression option
    nvtt::CompressionOptions compressionOptions;
    compressionOptions.setFormat(nvttFormat );

    if (nvttFormat == nvtt::Format_BC2)
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
    else if (nvttFormat == nvtt::Format_BC1a)
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
    else if (nvttFormat == nvtt::Format_BC6U)
    {
      compressionOptions.setPixelType(nvtt::PixelType_UnsignedFloat);
    }
    else if (nvttFormat == nvtt::Format_BC6S)
    {
      compressionOptions.setPixelType(nvtt::PixelType_Float);
    }

    if (alpha && options.enableAlphaDithering &&
        nvttFormat != nvtt::Format_BC2 &&
        nvttFormat != nvtt::Format_BC1a)
    {
      unsigned char bits = 8;
      compressionOptions.setPixelFormat(8, 8, 8, bits);
      compressionOptions.setQuantization(/*color dithering*/ false,
                                         /*alpha dithering*/ true,
                                         /*binary alpha*/ false);
    }

    compressionOptions.setQuality(options.quality);

		if (bc1n)
    {
      compressionOptions.setColorWeights(1.f, 1.f, 0);
    }

    // Automatically use dds10 if compressing to BC6 or BC7
    if (nvttFormat == nvtt::Format_BC6U ||
        nvttFormat == nvtt::Format_BC6S ||
        nvttFormat == nvtt::Format_BC7)
    {
      dds10 = true;
    }

    nvtt::Context context(options.useGPU);

    nvtt::Surface image;
    nvtt::SurfaceSet images;

		bool multiInputImage{false};
    nvtt::TextureType textureType{nvtt::TextureType_2D};

		if (options.maxMipmapCount > 1)
    {
      if (images.loadDDSFromMemory(input.data(), input.size()))
      {
        textureType = images.GetTextureType();
        image = images.GetSurface(0, 0);
        multiInputImage =
            (images.GetMipmapCount() > 1 || images.GetFaceCount() > 1);
			}
		}

		if (image.isNull())
    {
      if (!image.loadFromMemory(input.data(), input.size(), nullptr))
      {
        return false;
      }
      textureType = image.type();
		}

		nvtt::AlphaMode alphaMode = data.alphaMode;
    image.setNormalMap(normal);

    if (nvttFormat == nvtt::Format_BC6U || nvttFormat == nvtt::Format_BC6S)
      alphaMode = nvtt::AlphaMode_None;

    image.setAlphaMode(alphaMode);

    const int faceCount = multiInputImage ? images.GetFaceCount() : 1;

    const int mip0Width = image.width();
    const int mip0Height = image.height();
    const int mip0Depth = image.depth();
    // How many mipmaps, including the base mip, will we generate?
    int mipmapCount = 0;
    while (mipmapCount < options.maxMipmapCount)
    {
      // Look at the next mipmap's size. Does it satisfy our
      // constraints?
      const int mipWidth = (std::max)(1, mip0Width >> mipmapCount);
      const int mipHeight = (std::max)(1, mip0Height >> mipmapCount);
      if ((mipWidth < options.minMipmapSize) ||
          (mipHeight < options.minMipmapSize))
      {
        break;
      }
      mipmapCount++; // Accept it.
      if (mipWidth == 1 && mipHeight == 1)
      {
        break; // Stop generating mips.
      }
    }
    // mipmapCount can be 0 here if mip 0 was smaller than minMipSize.
    mipmapCount = (std::max)(1, mipmapCount);

    std::unique_ptr<PathOutputHandler> outputHandler =
        std::make_unique<PathOutputHandler>(exportPath);
    std::unique_ptr<nvtt::OutputOptions> outputOptions =
        std::make_unique<nvtt::OutputOptions>();
    outputOptions->setOutputHandler(outputHandler.get());

    if (dds10)
    {
      outputOptions->setContainer(nvtt::Container_DDS10);
    }

    //// compress procedure

		// We split apart batches before they get too large.
    // `batchSizeLimit` limits the total input file size in multi-file
    // batches. Batch compression is faster than compressing each file
    // one-by-one, because the GPU can compress all the files in parallel.
    // However, when there's a lot of data to compress, we might run out of
    // memory. So, we try to stop before we get to that point.
    // Note that this method of limiting the total input file size is
    // imperfect: a PNG file could be small, but contain a lot of pixel data.
    const uintmax_t batchInputSizeLimit = 104857600;
    uintmax_t curBatchInputSize = 0;

    // Windows' C runtime limits the number of open files to 512 by default.
    // Because the default OutputHandler keeps a file open until it is
    // destroyed, we limit batches to at most 500 files (we use a value lower
    // than 512 to provide a safety buffer).
    const unsigned batchFileLimit = 500;
    unsigned curBatchFiles = 0;

    nvtt::BatchList batchList;
    std::vector<std::unique_ptr<nvtt::Surface>> SurfaceList;

    if (!context.outputHeader(textureType, mip0Width, mip0Height, mip0Depth,
                              mipmapCount, normal,
                              compressionOptions, *outputOptions))
    {
      return false;
    }

    // Output images.
    for (int f = 0; f < faceCount; f++)
    {
      // Can we use the input SurfaceSet (true)? Or do we have to regenerate
      // mipmaps (false)?
      bool useSourceImages = true;

      if (f > 0)
        images.GetSurface(f, 0, image);

      // To linear space.
      if (!image.isNormalMap() && (mipmapCount > 1) && options.enableGammaCorrect)
      {
        image.toLinearFromSrgbUnclamped();
      }

      nvtt::Surface tmp = image;
      if (!tmp.isNormalMap() && (mipmapCount > 1) && options.enableGammaCorrect)
      {
        tmp.toSrgbUnclamped();
      }

      context.quantize(tmp, compressionOptions);
      std::unique_ptr<nvtt::Surface> surf =
          std::make_unique<nvtt::Surface>(tmp);
      batchList.Append(surf.get(), f, 0, outputOptions.get());
      SurfaceList.push_back(std::move(surf));

      for (int m = 1; m < mipmapCount; m++)
      {
        if (useSourceImages)
        {
          if (!multiInputImage || m >= images.GetMipmapCount())
          { // One face is missing in this mipmap level.
            useSourceImages = false; // If one level is missing, ignore the
                                     // following source images.
          }
        }

        if (useSourceImages)
        {
          images.GetSurface(f, m, image);
          // For already generated mipmaps, we need to convert to linear.
          if (!image.isNormalMap() && options.enableGammaCorrect)
          {
            image.toLinearFromSrgbUnclamped();
          }
        }
        else
        {
          if (options.mipmapFilter == nvtt::MipmapFilter_Kaiser)
          {
            float params[2] = {1.0f /*kaiserStretch*/, 4.0f /*kaiserAlpha*/};
            image.buildNextMipmap(nvtt::MipmapFilter_Kaiser, 3 /*kaiserWidth*/,
                                  params, 1);
          }
          else
          {
            image.buildNextMipmap(options.mipmapFilter, 1);
          }
        }

        if (image.isNormalMap())
        {
          if (data.isNormalMap)
          {
            image.normalizeNormalMap();
          }
          tmp = image;
        }
        else
        {
          tmp = image;
          if (options.enableGammaCorrect)
          {
            tmp.toSrgbUnclamped();
          }
        }

        context.quantize(tmp, compressionOptions);
        std::unique_ptr<nvtt::Surface> surf =
            std::make_unique<nvtt::Surface>(tmp);
        batchList.Append(surf.get(), f, m, outputOptions.get());
        SurfaceList.push_back(std::move(surf));
      }
    }

		const bool compressedOK = context.compress(batchList, compressionOptions);
    if (!compressedOK)
    {
      return false;
    }

		batchList.Clear();
    SurfaceList.clear();

		return true;
	}
  else
  {
		// Fetch data from the memory
    int width, height, channels, ok;
    // Check if the file is a valid texture
    ok = stbi_info_from_memory((uint8_t*)input.data(), input.size(), &width, &height, &channels);
    if (!ok)
    {
      return false;
    }

		const int requiredChannels = GetNumChannels(options.format);

		/*const bool hasAlpha = (channels == 4);
		const bool isHdr =
        stbi_is_hdr_from_memory((uint8_t*)input.data(), input.size());
    const bool is16bits =
        stbi_is_16_bit_from_memory((uint8_t*)input.data(), input.size());*/

		nvtt::InputFormat inputFormat;

		// Uncompress -> Uncompress
		if (IsUncompressedFormat(options.format))
    {
			// cubemap conversion
      if (options.convertToCubeMap)
      {
        nvtt::Surface surface;
        surface.loadFromMemory((void*)input.data(), input.size());

				nvtt::CubeSurface cube;
        cube.fold(surface, options.cubeLayout);

				const bool alpha = (data.alphaMode != nvtt::AlphaMode_None);
        const bool normal = data.isNormalMap;

        bool dds10{true};

        nvtt::Surface image;

        bool multiInputImage{true};
        nvtt::TextureType textureType{nvtt::TextureType_2D};

				image = cube.face(0);

				textureType = image.type();

        nvtt::AlphaMode alphaMode = data.alphaMode;
        image.setNormalMap(normal);
        image.setAlphaMode(alphaMode);

        const int faceCount = 6;

        const int mip0Width = image.width();
        const int mip0Height = image.height();
        const int mip0Depth = image.depth();
        // How many mipmaps, including the base mip, will we generate?
        int mipmapCount = 0;
        while (mipmapCount < options.maxMipmapCount)
        {
          // Look at the next mipmap's size. Does it satisfy our
          // constraints?
          const int mipWidth = (std::max)(1, mip0Width >> mipmapCount);
          const int mipHeight = (std::max)(1, mip0Height >> mipmapCount);
          if ((mipWidth < options.minMipmapSize) ||
              (mipHeight < options.minMipmapSize))
          {
            break;
          }
          mipmapCount++; // Accept it.
          if (mipWidth == 1 && mipHeight == 1)
          {
            break; // Stop generating mips.
          }
        }
        // mipmapCount can be 0 here if mip 0 was smaller than minMipSize.
        mipmapCount = (std::max)(1, mipmapCount);

				// TODO: Save the cubemap
        const bool isHdr = IsHDR(options.format);
        const bool is16Bits = Is16Bits(options.format);

				const size_t elemSize = !isHdr ? 1 : is16Bits ? 2 : 4;

        nv_dds::Image dds;
        dds.allocate(mipmapCount, 1, faceCount);
        size_t linearSize = mip0Width * mip0Height * requiredChannels;

				size_t originalSize = mip0Width * mip0Height * 4;
        
				for (int f = 0; f < faceCount; ++f)
        {
          nvtt::Surface& tmp = cube.face(f);

					if (!isHdr)
          {
            std::vector<uint8_t> pixels(linearSize);
            for (size_t y = 0; y < mip0Height; ++y)
            {
              for (size_t x = 0; x < mip0Width; ++x)
              {
                size_t currPixelIndex = y * mip0Width + x;
                pixels[currPixelIndex + 0] =
                    tmp.channel(0)[currPixelIndex] / (tmp.channel(
                        0)[currPixelIndex] + 1.f);
                if (requiredChannels > 1)
                  pixels[currPixelIndex + 1] =
                      tmp.channel(1)[currPixelIndex] /
                      (tmp.channel(1)[currPixelIndex] + 1.f);
                if (requiredChannels > 2)
                  pixels[currPixelIndex + 2] =
                      tmp.channel(2)[currPixelIndex] /
                      (tmp.channel(2)[currPixelIndex] + 1.f);
                if (requiredChannels > 3)
                  pixels[currPixelIndex + 3] =
                      tmp.channel(3)[currPixelIndex] /
                      (tmp.channel(3)[currPixelIndex] + 1.f);
              }
            }
            // TODO: Convert the original pixels into desired pixels
            dds.subresource(0, 0, f).create(linearSize * elemSize,
                                            pixels.data());
          }
          else
          {
            if (is16Bits)
            {
              std::vector<uint16_t> pixels(linearSize);
              for (size_t y = 0; y < mip0Height; ++y)
              {
                for (size_t x = 0; x < mip0Width; ++x)
                {
                  size_t currPixelIndex = y * mip0Width + x;
                  memcpy(&pixels[currPixelIndex + 0], &tmp.channel(0)[currPixelIndex], 2);
                  if (requiredChannels > 1)
                    memcpy(&pixels[currPixelIndex + 1],
                           &tmp.channel(1)[currPixelIndex], 2);
                  if (requiredChannels > 2)
                    memcpy(&pixels[currPixelIndex + 2],
                           &tmp.channel(2)[currPixelIndex], 2);
                  if (requiredChannels > 3)
                    memcpy(&pixels[currPixelIndex + 3],
                           &tmp.channel(3)[currPixelIndex], 2);
                }
              }
              // TODO: Convert the original pixels into desired pixels
              dds.subresource(0, 0, f).create(linearSize * elemSize,
                                              pixels.data());
						}
            else
            {
              std::vector<float> pixels(linearSize);
              for (size_t y = 0; y < mip0Height; ++y)
              {
                for (size_t x = 0; x < mip0Width; ++x)
                {
                  size_t currPixelIndex = y * mip0Width + x;
                  pixels[currPixelIndex + 0] = tmp.channel(0)[currPixelIndex];
                  if (requiredChannels > 1)
                    pixels[currPixelIndex + 1] = tmp.channel(1)[currPixelIndex];
                  if (requiredChannels > 2)
                    pixels[currPixelIndex + 2] = tmp.channel(2)[currPixelIndex];
                  if (requiredChannels > 3)
                    pixels[currPixelIndex + 3] = tmp.channel(3)[currPixelIndex];
                }
              }
              // TODO: Convert the original pixels into desired pixels
              dds.subresource(0, 0, f).create(linearSize * elemSize,
                                              pixels.data());
						}
					}
				}
				
        // Output images.
        for (int f = 0; f < faceCount; f++)
        {
          for (int m = 1; m < mipmapCount; m++)
          {
            if (options.mipmapFilter == nvtt::MipmapFilter_Kaiser)
            {
              float params[2] = {1.0f /*kaiserStretch*/,
                                  4.0f /*kaiserAlpha*/};
              cube.face(f).buildNextMipmap(nvtt::MipmapFilter_Kaiser,
                                    3 /*kaiserWidth*/, params, 1);
            }
            else
            {
              cube.face(f).buildNextMipmap(options.mipmapFilter, 1);
            }

						nvtt::Surface& tmp = cube.face(f);
            size_t mipHeight = tmp.height();
            size_t mipWidth = tmp.width();

						size_t linearSize = mipWidth * mipHeight * requiredChannels;

            size_t originalSize = mipWidth * mipHeight * 4;

            if (!isHdr)
            {
              std::vector<uint8_t> pixels(linearSize);
              for (size_t y = 0; y < mipHeight; ++y)
              {
                for (size_t x = 0; x < mipWidth; ++x)
                {
                  size_t currPixelIndex = y * mipWidth + x;
                  pixels[currPixelIndex + 0] =
                      tmp.channel(0)[currPixelIndex] /
                      (tmp.channel(0)[currPixelIndex] + 1.f);
                  if (requiredChannels > 1)
                    pixels[currPixelIndex + 1] =
                        tmp.channel(1)[currPixelIndex] /
                        (tmp.channel(1)[currPixelIndex] + 1.f);
                  if (requiredChannels > 2)
                    pixels[currPixelIndex + 2] =
                        tmp.channel(2)[currPixelIndex] /
                        (tmp.channel(2)[currPixelIndex] + 1.f);
                  if (requiredChannels > 3)
                    pixels[currPixelIndex + 3] =
                        tmp.channel(3)[currPixelIndex] /
                        (tmp.channel(3)[currPixelIndex] + 1.f);
                }
              }
              // TODO: Convert the original pixels into desired pixels
              dds.subresource(m, 0, f).create(linearSize * elemSize,
                                              pixels.data());
            }
            else
            {
              if (is16Bits)
              {
                std::vector<uint16_t> pixels(linearSize);
                for (size_t y = 0; y < mipHeight; ++y)
                {
                  for (size_t x = 0; x < mipWidth; ++x)
                  {
                    size_t currPixelIndex = y * mipWidth + x;
                    memcpy(&pixels[currPixelIndex + 0],
                           &tmp.channel(0)[currPixelIndex], 2);
                    if (requiredChannels > 1)
                      memcpy(&pixels[currPixelIndex + 1],
                             &tmp.channel(1)[currPixelIndex], 2);
                    if (requiredChannels > 2)
                      memcpy(&pixels[currPixelIndex + 2],
                             &tmp.channel(2)[currPixelIndex], 2);
                    if (requiredChannels > 3)
                      memcpy(&pixels[currPixelIndex + 3],
                             &tmp.channel(3)[currPixelIndex], 2);
                  }
                }
                // TODO: Convert the original pixels into desired pixels
                dds.subresource(m, 0, f).create(linearSize * elemSize,
                                                pixels.data());
              }
              else
              {
                std::vector<float> pixels(linearSize);
                for (size_t y = 0; y < mipHeight; ++y)
                {
                  for (size_t x = 0; x < mipWidth; ++x)
                  {
                    size_t currPixelIndex = y * mipWidth + x;
                    pixels[currPixelIndex + 0] = tmp.channel(0)[currPixelIndex];
                    if (requiredChannels > 1)
                      pixels[currPixelIndex + 1] =
                          tmp.channel(1)[currPixelIndex];
                    if (requiredChannels > 2)
                      pixels[currPixelIndex + 2] =
                          tmp.channel(2)[currPixelIndex];
                    if (requiredChannels > 3)
                      pixels[currPixelIndex + 3] =
                          tmp.channel(3)[currPixelIndex];
                  }
                }
                // TODO: Convert the original pixels into desired pixels
                dds.subresource(m, 0, f).create(linearSize * elemSize, pixels.data());
              }
            }
          }
        }

				dds.mip0Width = mip0Width;
        dds.mip0Height = mip0Height;
        dds.mip0Depth = 1;
        dds.cubemapFaceFlags = nv_dds::DDSCAPS2_CUBEMAP_ALL_FACES;
        dds.dxgiFormat =
            todx(options.format, data.colorSpace == ColorSpace::kSRGB);
        dds.alphaMode = data.alphaMode == nvtt::AlphaMode_None
                            ? nv_dds::DDS_ALPHA_MODE_OPAQUE
                            : nv_dds::DDS_ALPHA_MODE_STRAIGHT;
        dds.isNormal = data.isNormalMap;

				dds.writeToFile(exportPath.c_str(), {.useDx10HeaderIfPossible = true});

        return true;
      }
      else
      {
        void* image;

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
        dds.dxgiFormat =
            todx(options.format, data.colorSpace == ColorSpace::kSRGB);
        dds.alphaMode = data.alphaMode == nvtt::AlphaMode_None
                            ? nv_dds::DDS_ALPHA_MODE_OPAQUE
                            : nv_dds::DDS_ALPHA_MODE_STRAIGHT;
        dds.isNormal = data.isNormalMap;

        dds.writeToFile(exportPath.c_str(), {.useDx10HeaderIfPossible = true});

				stbi_image_free(image);
        return true;
			}
		}
		// Uncompress -> Compress
    else
    {
      if (options.convertToCubeMap)
      {
        nvtt::Surface surface;
        surface.loadFromMemory((void*)input.data(), input.size());

        nvtt::CubeSurface cube;
        cube.fold(surface, options.cubeLayout);

        const bool alpha = (data.alphaMode != nvtt::AlphaMode_None);
        const bool normal = data.isNormalMap;
        const nvtt::Format nvttFormat = tonv(options.format);
        const bool bc1n = (options.format == ImageFormat::BC1) && normal;

        bool dds10{true};

        // Conpression option
        nvtt::CompressionOptions compressionOptions;
        compressionOptions.setFormat(nvttFormat);

        if (nvttFormat == nvtt::Format_BC2)
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
        else if (nvttFormat == nvtt::Format_BC1a)
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
        else if (nvttFormat == nvtt::Format_BC6U)
        {
          compressionOptions.setPixelType(nvtt::PixelType_UnsignedFloat);
        }
        else if (nvttFormat == nvtt::Format_BC6S)
        {
          compressionOptions.setPixelType(nvtt::PixelType_Float);
        }

        if (alpha && options.enableAlphaDithering &&
            nvttFormat != nvtt::Format_BC2 && nvttFormat != nvtt::Format_BC1a)
        {
          unsigned char bits = 8;
          compressionOptions.setPixelFormat(8, 8, 8, bits);
          compressionOptions.setQuantization(/*color dithering*/ false,
                                             /*alpha dithering*/ true,
                                             /*binary alpha*/ false);
        }

        compressionOptions.setQuality(options.quality);

        if (bc1n)
        {
          compressionOptions.setColorWeights(1.f, 1.f, 0);
        }

        // Automatically use dds10 if compressing to BC6 or BC7
        if (nvttFormat == nvtt::Format_BC6U ||
            nvttFormat == nvtt::Format_BC6S || nvttFormat == nvtt::Format_BC7)
        {
          dds10 = true;
        }

        nvtt::Context context(options.useGPU);

        nvtt::Surface image;

        bool multiInputImage{true};
        nvtt::TextureType textureType{nvtt::TextureType_2D};

        image = cube.face(0);

        textureType = image.type();

        nvtt::AlphaMode alphaMode = data.alphaMode;
        image.setNormalMap(normal);

        if (nvttFormat == nvtt::Format_BC6U || nvttFormat == nvtt::Format_BC6S)
          alphaMode = nvtt::AlphaMode_None;

        image.setAlphaMode(alphaMode);

        const int faceCount = 6;

        const int mip0Width = image.width();
        const int mip0Height = image.height();
        const int mip0Depth = image.depth();
        // How many mipmaps, including the base mip, will we generate?
        int mipmapCount = 0;
        while (mipmapCount < options.maxMipmapCount)
        {
          // Look at the next mipmap's size. Does it satisfy our
          // constraints?
          const int mipWidth = (std::max)(1, mip0Width >> mipmapCount);
          const int mipHeight = (std::max)(1, mip0Height >> mipmapCount);
          if ((mipWidth < options.minMipmapSize) ||
              (mipHeight < options.minMipmapSize))
          {
            break;
          }
          mipmapCount++; // Accept it.
          if (mipWidth == 1 && mipHeight == 1)
          {
            break; // Stop generating mips.
          }
        }
        // mipmapCount can be 0 here if mip 0 was smaller than minMipSize.
        mipmapCount = (std::max)(1, mipmapCount);

        std::unique_ptr<PathOutputHandler> outputHandler =
            std::make_unique<PathOutputHandler>(exportPath);
        std::unique_ptr<nvtt::OutputOptions> outputOptions =
            std::make_unique<nvtt::OutputOptions>();
        outputOptions->setOutputHandler(outputHandler.get());

        if (dds10)
        {
          outputOptions->setContainer(nvtt::Container_DDS10);
        }

        //// compress procedure

        // We split apart batches before they get too large.
        // `batchSizeLimit` limits the total input file size in multi-file
        // batches. Batch compression is faster than compressing each file
        // one-by-one, because the GPU can compress all the files in parallel.
        // However, when there's a lot of data to compress, we might run out of
        // memory. So, we try to stop before we get to that point.
        // Note that this method of limiting the total input file size is
        // imperfect: a PNG file could be small, but contain a lot of pixel
        // data.
        const uintmax_t batchInputSizeLimit = 104857600;
        uintmax_t curBatchInputSize = 0;

        // Windows' C runtime limits the number of open files to 512 by default.
        // Because the default OutputHandler keeps a file open until it is
        // destroyed, we limit batches to at most 500 files (we use a value
        // lower than 512 to provide a safety buffer).
        const unsigned batchFileLimit = 500;
        unsigned curBatchFiles = 0;

        nvtt::BatchList batchList;
        std::vector<std::unique_ptr<nvtt::Surface>> SurfaceList;

        if (!context.outputHeader(textureType, mip0Width, mip0Height, mip0Depth,
                                  mipmapCount, normal, compressionOptions,
                                  *outputOptions))
        {
          return false;
        }

        // Output images.
        for (int f = 0; f < faceCount; f++)
        {
          if (f > 0)
            image = cube.face(f);

          // To linear space.
          if (!image.isNormalMap() && (mipmapCount > 1) &&
              options.enableGammaCorrect)
          {
            image.toLinearFromSrgbUnclamped();
          }

          nvtt::Surface tmp = image;
          if (!tmp.isNormalMap() && (mipmapCount > 1) &&
              options.enableGammaCorrect)
          {
            tmp.toSrgbUnclamped();
          }

          context.quantize(tmp, compressionOptions);
          std::unique_ptr<nvtt::Surface> surf =
              std::make_unique<nvtt::Surface>(tmp);
          batchList.Append(surf.get(), f, 0, outputOptions.get());
          SurfaceList.push_back(std::move(surf));

          for (int m = 1; m < mipmapCount; m++)
          {

            if (options.mipmapFilter == nvtt::MipmapFilter_Kaiser)
            {
              float params[2] = {1.0f /*kaiserStretch*/, 4.0f /*kaiserAlpha*/};
              image.buildNextMipmap(nvtt::MipmapFilter_Kaiser,
                                    3 /*kaiserWidth*/, params, 1);
            }
            else
            {
              image.buildNextMipmap(options.mipmapFilter, 1);
            }

            if (image.isNormalMap())
            {
              if (data.isNormalMap)
              {
                image.normalizeNormalMap();
              }
              tmp = image;
            }
            else
            {
              tmp = image;
              if (options.enableGammaCorrect)
              {
                tmp.toSrgbUnclamped();
              }
            }

            context.quantize(tmp, compressionOptions);
            std::unique_ptr<nvtt::Surface> surf =
                std::make_unique<nvtt::Surface>(tmp);
            batchList.Append(surf.get(), f, m, outputOptions.get());
            SurfaceList.push_back(std::move(surf));
          }
        }

        const bool compressedOK =
            context.compress(batchList, compressionOptions);
        if (!compressedOK)
        {
          return false;
        }

        batchList.Clear();
        SurfaceList.clear();

        return true;
			
			}
      else
      {
        const bool alpha = (data.alphaMode != nvtt::AlphaMode_None);
        const bool normal = data.isNormalMap;
        const nvtt::Format nvttFormat = tonv(options.format);
        const bool bc1n = (options.format == ImageFormat::BC1) && normal;

        bool dds10{true};

        // Conpression option
        nvtt::CompressionOptions compressionOptions;
        compressionOptions.setFormat(nvttFormat);

        if (nvttFormat == nvtt::Format_BC2)
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
        else if (nvttFormat == nvtt::Format_BC1a)
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
        else if (nvttFormat == nvtt::Format_BC6U)
        {
          compressionOptions.setPixelType(nvtt::PixelType_UnsignedFloat);
        }
        else if (nvttFormat == nvtt::Format_BC6S)
        {
          compressionOptions.setPixelType(nvtt::PixelType_Float);
        }

        if (alpha && options.enableAlphaDithering &&
            nvttFormat != nvtt::Format_BC2 && nvttFormat != nvtt::Format_BC1a)
        {
          unsigned char bits = 8;
          compressionOptions.setPixelFormat(8, 8, 8, bits);
          compressionOptions.setQuantization(/*color dithering*/ false,
                                             /*alpha dithering*/ true,
                                             /*binary alpha*/ false);
        }

        compressionOptions.setQuality(options.quality);

        if (bc1n)
        {
          compressionOptions.setColorWeights(1.f, 1.f, 0);
        }

        // Automatically use dds10 if compressing to BC6 or BC7
        if (nvttFormat == nvtt::Format_BC6U ||
            nvttFormat == nvtt::Format_BC6S || nvttFormat == nvtt::Format_BC7)
        {
          dds10 = true;
        }

        nvtt::Context context(options.useGPU);

        nvtt::Surface image;

        bool multiInputImage{false};
        nvtt::TextureType textureType{nvtt::TextureType_2D};

        if (!image.loadFromMemory(input.data(), input.size(), nullptr))
        {
          return false;
        }
        textureType = image.type();

        nvtt::AlphaMode alphaMode = data.alphaMode;
        image.setNormalMap(normal);

        if (nvttFormat == nvtt::Format_BC6U || nvttFormat == nvtt::Format_BC6S)
          alphaMode = nvtt::AlphaMode_None;

        image.setAlphaMode(alphaMode);

        const int mip0Width = image.width();
        const int mip0Height = image.height();
        const int mip0Depth = 1;
        // How many mipmaps, including the base mip, will we generate?
        int mipmapCount = 0;
        while (mipmapCount < options.maxMipmapCount)
        {
          // Look at the next mipmap's size. Does it satisfy our
          // constraints?
          const int mipWidth = (std::max)(1, mip0Width >> mipmapCount);
          const int mipHeight = (std::max)(1, mip0Height >> mipmapCount);
          if ((mipWidth < options.minMipmapSize) ||
              (mipHeight < options.minMipmapSize))
          {
            break;
          }
          mipmapCount++; // Accept it.
          if (mipWidth == 1 && mipHeight == 1)
          {
            break; // Stop generating mips.
          }
        }
        // mipmapCount can be 0 here if mip 0 was smaller than minMipSize.
        mipmapCount = (std::max)(1, mipmapCount);

        std::unique_ptr<PathOutputHandler> outputHandler =
            std::make_unique<PathOutputHandler>(exportPath);
        std::unique_ptr<nvtt::OutputOptions> outputOptions =
            std::make_unique<nvtt::OutputOptions>();
        outputOptions->setOutputHandler(outputHandler.get());

        if (dds10)
        {
          outputOptions->setContainer(nvtt::Container_DDS10);
        }

        //// compress procedure

        // We split apart batches before they get too large.
        // `batchSizeLimit` limits the total input file size in multi-file
        // batches. Batch compression is faster than compressing each file
        // one-by-one, because the GPU can compress all the files in parallel.
        // However, when there's a lot of data to compress, we might run out of
        // memory. So, we try to stop before we get to that point.
        // Note that this method of limiting the total input file size is
        // imperfect: a PNG file could be small, but contain a lot of pixel
        // data.
        const uintmax_t batchInputSizeLimit = 104857600;
        uintmax_t curBatchInputSize = 0;

        // Windows' C runtime limits the number of open files to 512 by default.
        // Because the default OutputHandler keeps a file open until it is
        // destroyed, we limit batches to at most 500 files (we use a value
        // lower than 512 to provide a safety buffer).
        const unsigned batchFileLimit = 500;
        unsigned curBatchFiles = 0;

        nvtt::BatchList batchList;
        std::vector<std::unique_ptr<nvtt::Surface>> SurfaceList;

        if (!context.outputHeader(textureType, mip0Width, mip0Height, mip0Depth,
                                  mipmapCount, normal, compressionOptions,
                                  *outputOptions))
        {
          return false;
        }

        // Output images.

        // To linear space.
        if (!image.isNormalMap() && (mipmapCount > 1) &&
            options.enableGammaCorrect)
        {
          image.toLinearFromSrgbUnclamped();
        }

        nvtt::Surface tmp = image;
        if (!tmp.isNormalMap() && (mipmapCount > 1) &&
            options.enableGammaCorrect)
        {
          tmp.toSrgbUnclamped();
        }

        context.quantize(tmp, compressionOptions);
        std::unique_ptr<nvtt::Surface> surf =
            std::make_unique<nvtt::Surface>(tmp);
        batchList.Append(surf.get(), 0, 0, outputOptions.get());
        SurfaceList.push_back(std::move(surf));

        for (int m = 1; m < mipmapCount; m++)
        {

          if (options.mipmapFilter == nvtt::MipmapFilter_Kaiser)
          {
            float params[2] = {1.0f /*kaiserStretch*/, 4.0f /*kaiserAlpha*/};
            image.buildNextMipmap(nvtt::MipmapFilter_Kaiser, 3 /*kaiserWidth*/,
                                  params, 1);
          }
          else
          {
            image.buildNextMipmap(options.mipmapFilter, 1);
          }

          if (image.isNormalMap())
          {
            if (data.isNormalMap)
            {
              image.normalizeNormalMap();
            }
            tmp = image;
          }
          else
          {
            tmp = image;
            if (options.enableGammaCorrect)
            {
              tmp.toSrgbUnclamped();
            }
          }

          context.quantize(tmp, compressionOptions);
          std::unique_ptr<nvtt::Surface> surf =
              std::make_unique<nvtt::Surface>(tmp);
          batchList.Append(surf.get(), 0, m, outputOptions.get());
          SurfaceList.push_back(std::move(surf));
        }

        const bool compressedOK =
            context.compress(batchList, compressionOptions);
        if (!compressedOK)
        {
          return false;
        }

        batchList.Clear();
        SurfaceList.clear();

        return true;
			}
    }
	}

  return false;
}
