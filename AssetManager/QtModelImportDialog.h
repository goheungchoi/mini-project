#pragma once

#include <QDialog>
#include "ui_QtModelImportDialog.h"

class QtModelImportDialog : public QDialog
{
	Q_OBJECT

public:
	QtModelImportDialog(QWidget *parent = nullptr);
	~QtModelImportDialog();

private:
	Ui::QtModelImportDialogClass ui;
};
