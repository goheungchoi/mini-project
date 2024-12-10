#pragma once

#include <string>
#include <format>
#include <type_traits>

enum class ImageFormat
{
	BC7,
	BC6S,
	BC6U,
	BC5u,
	BC4u,
	BC3,
	BC3n,
	BC2,
	BC1a,
	BC1,
	R8_UINT,
	R8G8_UINT,
	R8G8B8_UINT,
	R8G8B8A8_UINT,
	R16_FLOAT,
	R16G16_FLOAT,
	R16G16B16_FLOAT,
	R16G16B16A16_FLOAT,
  R32_FLOAT,
  R32G32_FLOAT,
  R32G32B32_FLOAT,
  R32G326A32_FLOAT,
	kNumFormats
};

template<typename T>
  requires (std::is_enum_v<T>)
inline int eoti(T e) {
  return static_cast<int>(e);
}

template<typename T>
  requires(std::is_enum_v<T>)
inline T iote(int i) {
  return static_cast<T>(i);
}

constexpr std::tuple<const char*, const char*, int, const char*> imageFormatDetails[] = {
    /*  0 */ {"BC7", "RGBA", 8, "explicit alpha"},
    /*  1 */ {"BC6S", "HDR RGB", 8, "signed, no alpha"},
    /*  2 */ {"BC6U", "HDR RGB", 8, "unsigned, no alpha"},
    /*  3 */ {"BC5u", "RG", 8, "normal map"},
    /*  4 */ {"BC4u", "R", 4, "grayscale"},
    /*  5 */ {"BC3", "RGBA", 8, "DXT5: interpolated alpha"},
    /*  6 */ {"BC3n", "Normal", 8, "DXT5n: (x,y)->(1,y,0,x)"},
    /*  7 */ {"BC2", "RGBA", 8, "DXT3: explicit alpha"},
    /*  8 */ {"BC1a", "RGBA", 4, "DXT1a: 1 bit alpha"},
    /*  9 */ {"BC1", "RGB", 4, "DXT1: no alpha"},
    /* 10 */ {"8", "R", 8, "unsigned"},
    /* 11 */ {"8.8", "RG", 16, "unsigned"},
    /* 12 */ {"8.8.8", "RGB", 24, "unsigned"},
    /* 13 */ {"8.8.8.8", "RGBA", 32, "unsigned"},
    /* 14 */ {"16f", "R", 16, "floating-point"},
    /* 15 */ {"16fx2", "RG", 32, "floating-point"},
    /* 16 */ {"16fx3", "RGB", 32, "floating-point"},
    /* 17 */ {"16fx4", "RGBA", 64, "floating-point"},
    /* 18 */ {"32f", "R", 32, "floating-point"},
    /* 19 */ {"32fx2", "RG", 64, "floating-point"},
    /* 20 */ {"32fx3", "RGB", 64, "floating-point"},
    /* 21 */ {"32fx4", "RGBA", 128, "floating-point"}};

inline std::string ImageFormatToString(ImageFormat format) {
  int index = eoti(format);
  auto [name, values, numBits, description] = imageFormatDetails[index];
  return std::format("{:8} {:>8} {:>3} bpp | {}", name, values, numBits,
                     description);
}

inline std::string ImageFormatIndexToString(int index)
{
  auto [format, values, numBits, description] = imageFormatDetails[index];
  return std::format("{:8} {:>8} {:>3} bpp | {}", format, values, numBits,
                     description);
}