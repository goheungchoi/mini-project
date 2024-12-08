#pragma once

#include <QDialog>
#include <QGroupBox.h>
#include <QMessageBox.h>
#include "ui_TextureImportDialog.h"

#include <utility>
#include <filesystem>
namespace fs = std::filesystem;

class TextureImportDialog : public QDialog
{
	Q_OBJECT

public:
  TextureImportDialog(const QString& texturePath, QWidget* parent = nullptr);
  ~TextureImportDialog();

private:
  Ui::TextureImportDialogClass ui;


	QString _texturePath;
  std::pair<fs::path, fs::path> GetExportPaths(const QString& texturePath);

private slots:
  void onOKButtonClicked();

private:
  bool generateResourceFile();
};
