#include "TextureImportDialog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <nvtt/nvtt.h>

#include <format>
#include <QDir.h>

#include "UUID.h"

#include "TextureExport.h"

#include <filesystem>
#include <fstream>
#include <vector>
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

static QString ImageFormatToString(const char* format, const char* values, int numBits,
                            const char* explanation)
{
  return QString::fromStdString(std::format(
      "{:8} {:>8} {:>3} bpp | {}", format, values, numBits, explanation));
}

static QStringList imageFormats{
    /*  0 */ ImageFormatToString("BC7", "RGBA", 8, "explicit alpha"),
    /*  1 */ ImageFormatToString("BC6S", "HDR RGB", 8, "signed, no alpha"),
    /*  2 */ ImageFormatToString("BC6U", "HDR RGB", 8, "unsigned, no alpha"),
    /*  3 */ ImageFormatToString("BC5u", "RG", 8, "normal map"),
    /*  4 */ ImageFormatToString("BC4u", "R", 4, "grayscale"),
    /*  5 */ ImageFormatToString("BC3", "RGBA", 8, "DXT5: interpolated alpha"),
    /*  6 */ ImageFormatToString("BC3n", "Normal", 8, "DXT5n: (x,y)->(1,y,0,x)"),
    /*  7 */ ImageFormatToString("BC2", "RGBA", 8, "DXT3: explicit alpha"),
    /*  8 */ ImageFormatToString("BC1a", "RGBA", 4, "DXT1a: 1 bit alpha"),
    /*  9 */ ImageFormatToString("BC1", "RGB", 4, "DXT1: no alpha"),
    /* 10 */ ImageFormatToString("8", "R", 8, "unsigned"),
    /* 11 */ ImageFormatToString("8.8", "RG", 16, "unsigned"),
    /* 12 */ ImageFormatToString("8.8.8", "RGB", 24, "unsigned"),
    /* 13 */ ImageFormatToString("8.8.8.8", "RGBA", 32, "unsigned"),
    /* 14 */ ImageFormatToString("16f", "R", 16, "floating-point"),
    /* 15 */ ImageFormatToString("16fx2", "RG", 32, "floating-point"),
    /* 16 */ ImageFormatToString("16fx3", "RGB", 32, "floating-point"),
    /* 17 */ ImageFormatToString("16fx4", "RGBA", 64, "floating-point"),
    /* 18 */ ImageFormatToString("32f", "R", 32, "floating-point"),
    /* 19 */ ImageFormatToString("32fx2", "RG", 64, "floating-point"),
    /* 20 */ ImageFormatToString("32fx3", "RGB", 64, "floating-point"),
    /* 21 */ ImageFormatToString("32fx4", "RGBA", 128, "floating-point"),
};

TextureImportDialog::TextureImportDialog(const QString& texturePath, QWidget* parent)
    : QDialog(parent), _texturePath(texturePath)
{
	ui.setupUi(this);

	// Set up format combo box
  ui.formatComboBox->addItems(imageFormats);

	// Set up texture types
  ui.textureTypeComboBox->addItems({"2D Texture", "Cube Map"});

	// Set up image types
  ui.imageTypeComboBox->addItems(
      {"Color Map", "Grayscale", "Grayscale with Alpha", "Normal Map"});

	// Set up mipmap filters
  ui.filterTypeComboBox->addItems(
      {"Box", "Kaiser", "Triangle", "Mitchell-Netravali", "Min", "Max"});

	// Set up the transfer function
  ui.transferFunctionComboBox->addItems(
      {"Default", "sRGB", "Linear or Untagged"});
	
	// Bind the event actions
  connect(ui.cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(ui.okPushButton, &QPushButton::clicked, this,
          &TextureImportDialog::onOKButtonClicked);

	// Init
  readTextureFile();
}

TextureImportDialog::~TextureImportDialog() {


}

std::pair<fs::path, fs::path> TextureImportDialog::GetExportPaths(
    const QString& texturePath)
{
  QDir assetDir(ASSET_DIR);
  QString relativePath = assetDir.relativeFilePath(texturePath);

  UUID uuid = GenerateUUIDFromName(relativePath.toStdString());

  fs::path resourceDir(RESOURCE_DIR);
  resourceDir /= uuid.ToString().substr(0, 2);

	if (!fs::exists(resourceDir))
  {
    fs::create_directories(resourceDir);
	}

	fs::path resourcePath(resourceDir);
	resourcePath /= uuid.ToString();

	fs::path resourceInfoPath(resourcePath);
  resourceInfoPath /= ".info";

	return {resourcePath, resourceInfoPath};
}

bool TextureImportDialog::readTextureFile()
{
  std::string filename = _texturePath.toStdString();

  auto input = read_file(fs::path(filename));

  const bool isInputDDS =
      (input.size() >= 4 && input[0] == 'D' && input[1] == 'D' &&
       input[2] == 'S' && input[3] == ' ');

	if (isInputDDS)
  {
    QMessageBox::critical(this, "Error",
                          "DDS file format is not supported.");
	}

	// Get the texture information 
	int width, height, channels, ok;
  ok = stbi_info(filename.c_str(), &width, &height, &channels);
  if (!ok)
  {
    QMessageBox::critical(this, "Error", "Invalid image file!");
	}

	const bool isHdr = stbi_is_hdr(filename.c_str());
  const bool is16bits = stbi_is_16_bit(filename.c_str());
  if (isHdr)
  {
    if (channels == 1)
    {
      if (is16bits)
        ui.formatComboBox->setCurrentIndex(14);
      else
        ui.formatComboBox->setCurrentIndex(18);
		}
    else if (channels == 2)
    {
      if (is16bits)
        ui.formatComboBox->setCurrentIndex(15);
      else
        ui.formatComboBox->setCurrentIndex(19);
		}
    else if (channels == 3)
    {
      if (is16bits)
        ui.formatComboBox->setCurrentIndex(16);
      else
        ui.formatComboBox->setCurrentIndex(20);
		}
    else if (channels == 4)
    {
      if (is16bits)
        ui.formatComboBox->setCurrentIndex(17);
      else
        ui.formatComboBox->setCurrentIndex(21);
		}
  }
  else
  {
    if (channels == 1)
    {
        ui.formatComboBox->setCurrentIndex(10);
    }
    else if (channels == 2)
    {
        ui.formatComboBox->setCurrentIndex(11);
    }
    else if (channels == 3)
    {
        ui.formatComboBox->setCurrentIndex(12);
    }
    else if (channels == 4)
    {
        ui.formatComboBox->setCurrentIndex(13);
    }
	}

	if (channels == 1)
  {
    ui.imageTypeComboBox->setCurrentIndex(1);
  }
  else if (channels == 2)
  {
    ui.imageTypeComboBox->setCurrentIndex(2);
  }
  else if (channels == 3)
  {
    ui.imageTypeComboBox->setCurrentIndex(0);
  }
  else if (channels == 4)
  {
    ui.imageTypeComboBox->setCurrentIndex(0);
  }

  ui.sizeValueLabel->setText(
      QString::fromStdString(std::format("[ {}, {} ]", width, height)));

  return false;
}

bool TextureImportDialog::generateResourceFile()
{
	// TODO: Goheung Choi
  auto [exportPath, exportInfoPath] = GetExportPaths(_texturePath);


  ImageData data{};

  int formatIndex = ui.formatComboBox->currentIndex();
  if (formatIndex == 0)	// BC7
  {
  
	}

	CompressionOptions options{};

  return compress(_texturePath.toStdString(), exportPath.string(), data, options);
}


void TextureImportDialog::onOKButtonClicked()
{
  bool res = generateResourceFile();
  if (!res)
  {
    QMessageBox::critical(this, "Error", "Failed to generate texture resource file.");
  }
  else
  {
    accept();
	}
}