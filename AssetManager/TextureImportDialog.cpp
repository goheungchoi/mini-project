#include "TextureImportDialog.h"

#include <nvtt/nvtt.h>

#include <format>
#include <QDir.h>

#include "UUID.h"

#include "TextureExport.h"

static QString ImageFormatToString(const char* format, const char* values, int numBits,
                            const char* explanation)
{
  return QString::fromStdString(std::format(
      "{:8} {:>8} {:>3} bpp | {}", format, values, numBits, explanation));
}

static QStringList imageFormats{
    ImageFormatToString("BC7", "RGBA", 8, "explicit alpha"),
    ImageFormatToString("BC6S", "HDR RGB", 8, "signed, no alpha"),
    ImageFormatToString("BC6U", "HDR RGB", 8, "unsigned, no alpha"),
    ImageFormatToString("BC5u", "RG", 8, "normal map"),
    ImageFormatToString("BC4u", "R", 4, "grayscale"),
    ImageFormatToString("BC3", "RGBA", 8, "DXT5: interpolated alpha"),
    ImageFormatToString("BC3n", "Normal", 8, "DXT5n: (x,y)->(1,y,0,x)"),
    ImageFormatToString("BC2", "RGBA", 8, "DXT3: explicit alpha"),
    ImageFormatToString("BC1a", "RGBA", 4, "DXT1a: 1 bit alpha"),
    ImageFormatToString("BC1", "RGB", 4, "DXT1: no alpha"),
    ImageFormatToString("8", "A", 8, "alpha only"),
    ImageFormatToString("8", "L", 8, "luma only"),
    ImageFormatToString("8.8.8", "RGB", 24, "unsigned"),
    ImageFormatToString("8.8.8.8", "RGBA", 32, "unsigned"),
    ImageFormatToString("16f", "R", 16, "floating-point"),
    ImageFormatToString("16fx2", "RG", 32, "floating-point"),
    ImageFormatToString("16fx4", "RGBA", 64, "floating-point"),
    ImageFormatToString("32f", "R", 32, "floating-point"),
    ImageFormatToString("32fx2", "RG", 64, "floating-point"),
    ImageFormatToString("32fx4", "RGBA", 128, "floating-point"),
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