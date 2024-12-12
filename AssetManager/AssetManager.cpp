#include "AssetManager.h"

#include "TextureImportDialog.h"

#include <QMessageBox.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTableView>

#include <filesystem>
namespace fs = std::filesystem;

#include "UUID.h"

AssetManager::AssetManager(QWidget* parent)
    : QWidget(parent), assetDir(ASSET_DIR), resourceDir(RESOURCE_DIR)
{
  ui.setupUi(this);
  setFixedSize({800, 600});

	assetFilesModel = new QFileSystemModel();
  assetFilesModel->setRootPath(ASSET_DIR);

	// Set up the asset directory tree view
	ui.assetTreeView->setModel(assetFilesModel);
  ui.assetTreeView->setRootIndex(assetFilesModel->index(ASSET_DIR));
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

void AssetManager::loadResourceDetails(const QString& filePath) {
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
        extension == "dds" || extension == "hdr" || extension == "dds")
    {
      TextureImportDialog textureDialog(filePath, this);
      textureDialog.exec();
    }
    else if (extension == "obj" || extension == "fbx" || extension == "gltf")
    {
    
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

	fs::path resourceDir(RESOURCE_DIR);
  resourceDir /= uuid.ToString().substr(0, 2);

	QString message;
	if (fs::exists(resourceDir))
  {
    fs::path resourcePath(resourceDir);
    resourcePath /= uuid.ToString();
    if (fs::exists(resourcePath))
    {
      fs::path resourceInfoPath(resourcePath);
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