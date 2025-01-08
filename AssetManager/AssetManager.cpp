#include "AssetManager.h"

#include "TextureImportDialog.h"
#include "QtModelImportDialog.h"

#include "ModelExport.h"

#include <QMessageBox.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTableView>

#include "Shared/Config/Config.h"

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include "UUID.h"

AssetManager::AssetManager(QWidget* parent)
    : QWidget(parent)
{
  ui.setupUi(this);
  setFixedSize({800, 600});

	if (!SetDirectories())
  {
    QMessageBox::warning(this, "Project Path Error",
                         QString("Project path is can't be located:")
                             .append(fs::current_path().c_str()));
    return;
	}

	assetFilesModel = new QFileSystemModel();
  assetFilesModel->setRootPath(assetDir.absolutePath());

	// Set up the asset directory tree view
	ui.assetTreeView->setModel(assetFilesModel);
  ui.assetTreeView->setRootIndex(
      assetFilesModel->index(assetDir.absolutePath()));
  ui.assetTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

	// Set up the resource details model
  resourceDetailsModel = new QStandardItemModel(this);
  resourceDetailsModel->setHorizontalHeaderLabels({"Property", "Value"});

	// Set up the table view
  ui.resourceDetailsView->setModel(resourceDetailsModel);
  ui.resourceDetailsView->horizontalHeader()->setStretchLastSection(true);
  ui.resourceDetailsView->setColumnWidth(0, 120);

	// Bind event actions
	connect(ui.assetTreeView, &QTreeView::clicked, this,
          &AssetManager::onAssetSelected);
  connect(ui.importPushButton, &QPushButton::clicked, this,
          &AssetManager::onImportButtonClicked);
  connect(ui.excludePushButton, &QPushButton::clicked, this,
          &AssetManager::onExcludeButtonClicked);
}

AssetManager::~AssetManager() {
  delete assetFilesModel;
}

bool AssetManager::SetDirectories()
{
  fs::path cwd = fs::current_path();

	if (fs::exists(cwd / "Asset") && fs::exists(cwd / "Library")) {
    assetDir = fs::absolute(cwd / "Asset");
    resourceDir = fs::absolute(cwd / "Library" / "Resource");
    return true;
	}
	else {
    cwd /= "..";
    if (fs::exists(cwd / "Asset") && fs::exists(cwd / "Library")) {
      fs::current_path(cwd);
      assetDir = fs::absolute(cwd / "Asset");
      resourceDir = fs::absolute(cwd / "Library" / "Resource");
      return true;
		}
	}

	return false;
}

void AssetManager::loadResourceDetails(const QString& filePath)
{
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qWarning() << "Failed to open .info file:" << filePath;
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
  if (!jsonDoc.isObject())
  {
    qWarning() << "Invalid JSON format in .info file:" << filePath;
    return;
  }

  QJsonObject jsonObject = jsonDoc.object();
  resourceDetailsModel->clear();
  resourceDetailsModel->setHorizontalHeaderLabels({"Property", "Value"});

  for (auto key : jsonObject.keys())
  {
    QStandardItem* keyItem = new QStandardItem(key);
    QStandardItem* valueItem =
        new QStandardItem(jsonObject[key].toVariant().toString());
    resourceDetailsModel->insertRow(0, {keyItem, valueItem});
  }
}

static std::string GetExportPath(std::string path)
{
  std::string strUUID = GenerateUUIDFromName(path).ToString();

  fs::path resourceSubDir =
      fs::absolute(ns::kResourceDir) / strUUID.substr(0, 2);

  if (!fs::exists(resourceSubDir))
  {
    fs::create_directories(resourceSubDir);
  }

  fs::path exportPath = resourceSubDir / strUUID;

  return exportPath.string();
}

bool AssetManager::excludeAssetFileFromResource(const QString& filePath) {
  QString relativePath = assetDir.relativeFilePath(filePath);
  ui.assetPathLabel->setText(relativePath);

  fs::path fsRelativePath = relativePath.toStdString();
  std::string strRelativePath = fsRelativePath.string();
  std::replace(strRelativePath.begin(), strRelativePath.end(), '/', '\\');
  UUID uuid = GenerateUUIDFromName(strRelativePath);

  fs::path resDir(ns::kResourceDir);
	fs::path infoDir(ns::kResourceDir);
  resDir /= uuid.ToString().substr(0, 2);

  json j;
  if (fs::exists(resDir))
  {
    fs::path resourcePath(resDir);
    resDir /= uuid.ToString();
    if (fs::exists(resDir))
    {
      infoDir = resDir;
      infoDir += ".info";
      if (fs::exists(infoDir))
      {
        // Load the .info file
        std::ifstream fs(infoDir);
        j = json::parse(fs);
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
	
  if (j["resource_type"] == "texture")
  {
    fs::remove(resDir);
    fs::remove(infoDir);
  }
  else if (j["resource_type"] == "model")
  {
    json& details = j["details"];

		// Remove meshes
    for (const auto& mesh : details["meshes"])
    {
      fs::path exportPath = GetExportPath(mesh);
      fs::remove(exportPath);

			// Remove .info file
      exportPath += ".info";
      fs::remove(exportPath);
    }

		// Remove materials
    for (const auto& material : details["materials"])
    {
      fs::path exportPath = GetExportPath(material);
      fs::remove(exportPath);

      // Remove .info file
      exportPath += ".info";
      fs::remove(exportPath);
    }

    // Remove textures
    for (const auto& texture : details["textures"])
    {
      fs::path exportPath = GetExportPath(texture);
      fs::remove(exportPath);

      // Remove .info file
      exportPath += ".info";
      fs::remove(exportPath);
		}

		// Remove the model data
		fs::remove(resDir);
    fs::remove(infoDir);
  }
  else
  {
    return false;
  }

	return true;
}

void AssetManager::onImportButtonClicked() {
  QModelIndexList currSelectedAssetIndices = ui.assetTreeView->selectionModel()->selectedIndexes();
	
	if (currSelectedAssetIndices.size() > 0)
  {
    QModelIndex selectedIndex = currSelectedAssetIndices.at(0);
    QString filePath = assetFilesModel->filePath(selectedIndex);

		QString extension = QFileInfo(filePath).suffix().toLower();

		if (extension == "jpg" || extension == "jpeg" || extension == "png" ||
        extension == "dds" || extension == "hdr")
    {
      TextureImportDialog textureDialog(filePath, this);
      textureDialog.exec();
    }
    else if (extension == "obj" || extension == "fbx" || extension == "gltf" ||
             extension == "glm" || extension == "glb" || extension == "gltf2")
    {
      QtModelImportDialog modelDialog(assetDir, resourceDir, filePath, this);
      modelDialog.exec();
		}
    else
    {
      QMessageBox::warning(
          this, "Unsupported File Type",
          "The selected file type is not supported for import.");
		}

		onAssetSelected(ui.assetTreeView->currentIndex());
	}
  else
  {
    QMessageBox::warning(this, "No File Selected",
                         "Please select a file to import.");
    return;
	}
}

void AssetManager::onExcludeButtonClicked() {
  QModelIndexList currSelectedAssetIndices =
      ui.assetTreeView->selectionModel()->selectedIndexes();

  if (currSelectedAssetIndices.size() > 0)
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exclude Asset",
                          "Resource files will be removed permanently.\n"
                          "Are you sure you want to continue?",
													QMessageBox::Yes | QMessageBox::No);

		if (reply == QMessageBox::Yes)
    {
      QModelIndex selectedIndex = currSelectedAssetIndices.at(0);
      QString filePath = assetFilesModel->filePath(selectedIndex);

      if (excludeAssetFileFromResource(filePath))
      {
        QMessageBox::information(
            this, "Asset Exclusion Success",
            "The resource files of the selected asset is"
            " successfully removed from the resource list.");
      }
      else
      {
        QMessageBox::warning(this, "Asset Exclusion Failed",
                             "The resource files can't be removed due to"
                             " the following reasons:\n\n"
                             " - .info file is not found.\n"
                             " - Asset is not imported yet.\n"
                             " - Asset is in unsupported format.\n");
      }

			onAssetSelected(ui.assetTreeView->currentIndex());
		}
	}
  else
  {
    QMessageBox::warning(this, "No File Selected",
                         "Please select a file to import.");
  }
}


void AssetManager::onAssetSelected(const QModelIndex& index) {
  QString filePath = assetFilesModel->filePath(index);
  QString relativePath = assetDir.relativeFilePath(filePath);
  ui.assetPathLabel->setText(relativePath);

	fs::path fsRelativePath = relativePath.toStdString();
  std::string strRelativePath = fsRelativePath.string();
  std::replace(strRelativePath.begin(), strRelativePath.end(), '/', '\\');
  UUID uuid = GenerateUUIDFromName(strRelativePath);

	fs::path resDir(ns::kResourceDir);
  resDir /= uuid.ToString().substr(0, 2);

	QString message;
  if (fs::exists(resDir))
  {
    fs::path resourcePath(resDir);
    resDir /= uuid.ToString();
    if (fs::exists(resDir))
    {
      fs::path resourceInfoPath(resDir);
      resourceInfoPath += ".info";
      if (fs::exists(resourceInfoPath))
      {
				// Load the .info file to show details 
        QString qstr = QString::fromStdString(resourceInfoPath.string());
        loadResourceDetails(qstr);
				return;
      }
      else
      {
        message = "Information is not found";
			}
    }
    else
    {
      message = "Not imported yet";
		}
  }
  else
  {
    message = "Not imported yet";
	}

  resourceDetailsModel->clear();
  resourceDetailsModel->setHorizontalHeaderLabels({"Property", "Value"});
  QStandardItem* keyItem = new QStandardItem("Status");
  QStandardItem* valueItem = new QStandardItem(message);
  resourceDetailsModel->appendRow({keyItem, valueItem});
	
	qDebug() << "Selected File Relative Path: " << relativePath;
}