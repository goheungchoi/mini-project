#include "TextureImportDialog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define NV_DDS_UTILITY_VALUES
#include <nv_dds.h>
#include <nvtt/nvtt.h>

#include <format>
#include <QDir.h>

#include "UUID.h"

#include "TextureExport.h"

#include <filesystem>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;

#include "ImageFormatEnum.h"

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

TextureImportDialog::TextureImportDialog(const QString& texturePath, QWidget* parent)
    : QDialog(parent), _texturePath(texturePath)
{
	ui.setupUi(this);

	// Set up format combo box
  QStringList imageFormatList;
  for (int i = 0; i < eoti(ImageFormat::kNumFormats); ++i)
  {
    imageFormatList.append(QString::fromStdString(ImageFormatIndexToString(i)));
	}

  ui.formatComboBox->addItems(imageFormatList);

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

	// Set up the cube map layouts
  ui.cubeLayoutComboBox->addItems(
      {"Vertical Cross", "Horizontal Cross", "Column", "Row"});
	
	// Bind the event actions
  connect(ui.cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(ui.okPushButton, &QPushButton::clicked, this,
          &TextureImportDialog::onOKButtonClicked);

	// If the texture file is not supported format,
	// close the dialog
  if (!readTextureFile())
  {
    this->reject();
	}
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
    /*QMessageBox::critical(this, "Error",
                          "DDS file format is not supported.");*/
    nv_dds::Image dds;
    dds.readHeaderFromMemory(input.data(), input.size(), {});

		const nv_dds::DDSHeader& header = dds.getFileInfo().ddsh;

		if ((header.dwCaps2 & nv_dds::DDSCAPS2_CUBEMAP) != 0)
    {
      ui.textureTypeComboBox->setCurrentIndex(1);
		}

		ui.sizeValueLabel->setText(QString::fromStdString(
        std::format("[ {}, {} ]", header.dwWidth, header.dwHeight)));
    ui.mipLevelValueLabel->setText(QString::number(header.dwMipMapCount));

    return true;
  }
  else
  {
    // Get the texture information
    int width, height, channels, ok;
    ok = stbi_info_from_memory((uint8_t*)input.data(), input.size(), &width, &height, &channels);
    if (!ok)
    {
      QMessageBox::critical(this, "Error", "Invalid image file!");
      return false;
    }

    const bool isHdr =
        stbi_is_hdr_from_memory((uint8_t*)input.data(), input.size());
    const bool is16bits =
        stbi_is_16_bit_from_memory((uint8_t*)input.data(), input.size());
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
    ui.mipLevelValueLabel->setText(QString::number(1));

    return true;
	}
}

bool TextureImportDialog::generateResourceFile()
{
	// TODO: Goheung Choi
  auto [exportPath, exportInfoPath] = GetExportPaths(_texturePath);

	// Image data
  ImageData data{};
  data.isNormalMap = ui.imageTypeComboBox->currentText() == "Normal Map";
  data.isCubeMap = ui.textureTypeComboBox->currentText() == "Cube Map";

	if (ui.transferFunctionComboBox->currentText() == "sRGB")
  {
    data.colorSpace = ColorSpace::kSRGB;
	}
  else if (ui.transferFunctionComboBox->currentText() == "Linear or Untagged")
  {
    data.colorSpace = ColorSpace::kLinear;
	}
  else
  {
    if (ui.imageTypeComboBox->currentText() == "Color Map")
      data.colorSpace = ColorSpace::kSRGB;
    else
      data.colorSpace = ColorSpace::kLinear;
	}

	data.alphaMode = ui.premultipliedAlphaBlendingCheckBox->isChecked()
                       ? nvtt::AlphaMode_Transparency
                       : nvtt::AlphaMode_None;

	// Option data
  TextureExportOptions options;
  int formatIndex = ui.formatComboBox->currentIndex();
  options.format = iote<ImageFormat>(formatIndex);
  if (ui.fastestCheckBox->isChecked())
  {
    options.quality = nvtt::Quality_Fastest;
  }
  else if (ui.normalCheckBox->isChecked())
  {
    options.quality = nvtt::Quality_Normal;
  }
  else if (ui.productionCheckBox->isChecked())
  {
    options.quality = nvtt::Quality_Production;
  }
  else if (ui.highestCheckBox->isChecked())
  {
    options.quality = nvtt::Quality_Highest;
  }
	options.enableGammaCorrect = ui.gammaCorrectCheckBox->isChecked();
  options.enablePremultipliedAlphaBlending =
      ui.premultipliedAlphaBlendingCheckBox->isChecked();
  options.enableAlphaDithering = ui.alphaDitheringCheckBox->isChecked();
  options.enableMipmap = ui.maxMipmapCountSpinBox->value() != 1;
  options.maxMipmapCount = ui.maxMipmapCountSpinBox->value() == 0
                               ? (std::numeric_limits<int>::max)()
                               : ui.maxMipmapCountSpinBox->value();
  options.minMipmapSize = ui.minMipmapSizeSpinBox->value();
	if (ui.filterTypeComboBox->currentText() == "Box")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Box;
	}
  else if (ui.filterTypeComboBox->currentText() == "Kaiser")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Kaiser;
  }
  else if (ui.filterTypeComboBox->currentText() == "Triangle")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Triangle;
  }
  else if (ui.filterTypeComboBox->currentText() == "Mitchell-Netravali")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Mitchell;
  }
  else if (ui.filterTypeComboBox->currentText() == "Min")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Min;
  }
  else if (ui.filterTypeComboBox->currentText() == "Max")
  {
    options.mipmapFilter = nvtt::MipmapFilter_Max;
  }
  options.convertToCubeMap = ui.convertToCubeCheckBox->isChecked();
  if (ui.cubeLayoutComboBox->currentText() == "Vertical Cross")
  {
    options.cubeLayout = nvtt::CubeLayout_VerticalCross;
  }
  else if (ui.cubeLayoutComboBox->currentText() == "Horizontal Cross")
  {
    options.cubeLayout = nvtt::CubeLayout_HorizontalCross;
  }
  else if (ui.cubeLayoutComboBox->currentText() == "Column")
  {
    options.cubeLayout = nvtt::CubeLayout_Column;
  }
  else if (ui.cubeLayoutComboBox->currentText() == "Row")
  {
    options.cubeLayout = nvtt::CubeLayout_Row;
  }
	

  return textureExport(_texturePath.toStdString(), exportPath.string(), data, options);
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