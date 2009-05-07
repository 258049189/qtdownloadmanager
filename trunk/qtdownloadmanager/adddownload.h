#ifndef ADDDOWNLOAD_H
#define ADDDOWNLOAD_H

#include <QtGui/QWidget>
#include "ui_adddownload.h"
#include <QFileDialog>

class AddDownload : public QWidget
{
    Q_OBJECT

public:
    AddDownload(QWidget *parent = 0);
    ~AddDownload();

public slots:
	int AD_SaveDirToolButtonClicked();
	int AD_CancelButtonClicked();

public:
    Ui::AddDownloadClass ui;
};

#endif // ADDDOWNLOAD_H
