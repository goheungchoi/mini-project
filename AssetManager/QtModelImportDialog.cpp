#include "QtModelImportDialog.h"

#include "ModelExport.h"

#include <format>


QtModelImportDialog::QtModelImportDialog(const QDir& assetDir,
                                         const QDir& resourceDir,
                                         const QString& modelPath,
                                         QWidget* parent)
    : QDialog(parent), _assetDir(assetDir), _resourceDir(resourceDir),
      _modelPath(modelPath)
{
	ui.setupUi(this);

  // Bind the event actions
  connect(ui.cancelButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(ui.oKButton, &QPushButton::clicked, this,
          &QtModelImportDialog::onOKButtonClicked);
}

QtModelImportDialog::~QtModelImportDialog() {}

void QtModelImportDialog::onOKButtonClicked()
{
  QString extension = QFileInfo(_modelPath).suffix().toLower();

  ModelFileFormat fileformat{ModelFileFormat::kUnknown};
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

  ModelExporter exporter{_assetDir.filesystemPath(),
                         _resourceDir.filesystemPath()};
  bool res =
      exporter.ExportModel(_modelPath.toStdString().c_str(), fileformat,
                           ui.precalculateVertexPositionsCheckBox->isChecked(),
                           ui.extractArmatureDataCheckBox->isChecked(),
													 ui.exportAnimationsCheckBox->isChecked());
  if (!res)
  {
    QMessageBox::critical(this, "Error",
                          "Failed to generate texture resource file.");
  }
  else
  {
    accept();
  }
}
