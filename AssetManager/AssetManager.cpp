#include "AssetManager.h"

#include "TextureImportDialog.h"

#include "ModelExport.h"

#include <QMessageBox.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTableView>

#include <filesystem>
namespace fs = std::filesystem;

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
    resourceDetailsModel->appendRow({keyItem, valueItem});
  }
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
      ModelFileFormat fileformat;
      if (extension == "obj")
      {
        fileformat = ModelFileFormat::kOBJ;
      }
      else if (extension == "gltf" || extension == "gltf2" || extension == "glm" ||
               extension == "glb")
      {
        fileformat = ModelFileFormat::kGLTF;
      }
      else if (extension == "fbx")
      {
        fileformat = ModelFileFormat::kFBX;
			}

      ModelExporter exporter{assetDir.filesystemPath(),
                             resourceDir.filesystemPath()};
      exporter.ExportModel(filePath.toStdString().c_str(), fileformat);
		}
    else
    {
      QMessageBox::warning(
          this, "Unsupported File Type",
          "The selected file type is not supported for import.");
		}
	}
  else
  {
    QMessageBox::warning(this, "No File Selected",
                         "Please select a file to import.");
    return;
	}
}

void AssetManager::onModifyButtonClicked() {}


void AssetManager::onAssetSelected(const QModelIndex& index) {
  QString filePath = assetFilesModel->filePath(index);
  QString relativePath = assetDir.relativeFilePath(filePath);
  ui.assetPathLabel->setText(relativePath);

	UUID uuid = GenerateUUIDFromName(relativePath.toStdString());

	fs::path resDir(resourceDir.filesystemPath());
  resDir /= uuid.ToString().substr(0, 2);

	QString message;
  if (fs::exists(resDir))
  {
    fs::path resourcePath(resDir);
    resDir /= uuid.ToString();
    if (fs::exists(resDir))
    {
      fs::path resourceInfoPath(resDir);
      resourceInfoPath /= ".info";
      if (fs::exists(resourceInfoPath))
      {
				// Load the .info file to show details 
        QString qstr(resourceInfoPath.generic_string().c_str());
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