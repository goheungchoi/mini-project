#pragma once

#include "ui_AssetManager.h"
#include <QtWidgets/QWidget>
#include <QFileSystemModel.h>
#include <QStandardItemModel.h>

#include <QDir>

class AssetManager : public QWidget
{
  Q_OBJECT

public:
  AssetManager(QWidget* parent = nullptr);
  ~AssetManager();

private:
  QDir assetDir;
  QDir resourceDir;

	bool SetDirectories();

  QFileSystemModel* assetFilesModel;
  QStandardItemModel* resourceDetailsModel;

	void loadResourceDetails(const QString& filePath);

  Ui::AssetManagerClass ui;

private slots:
  void onAssetSelected(const QModelIndex& index);
  void onImportButtonClicked();
  void onModifyButtonClicked();
};
