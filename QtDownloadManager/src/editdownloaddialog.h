#ifndef EDITDOWNLOADDIALOG_H
#define EDITDOWNLOADDIALOG_H

#include <QtGui/QWidget>
#include "ui_editdownloaddialog.h"

class EditDownloadDialog : public QWidget
{
    Q_OBJECT

public:
    EditDownloadDialog(QWidget *parent = 0);
    ~EditDownloadDialog();

private:
    Ui::EditDownloadDialogClass ui;
};

#endif // EDITDOWNLOADDIALOG_H
