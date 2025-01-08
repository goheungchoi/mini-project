#pragma once

#include <QDir.h>
#include <QPushButton.h>

#include <QDialog>
#include <QGroupBox.h>
#include <QMessageBox.h>
#include "ui_QtModelImportDialog.h"

#include <filesystem>
#include <utility>
namespace fs = std::filesystem;

class QtModelImportDialog : public QDialog
{
	Q_OBJECT

public:
  QtModelImportDialog(const QDir& assetDir, const QDir& resourceDir,
                      const QString& modelPath, QWidget* parent = nullptr);
	~QtModelImportDialog();

private:
	Ui::QtModelImportDialogClass ui;

	QDir _assetDir, _resourceDir;
	QString _modelPath;

private slots:
  void onOKButtonClicked();
};
