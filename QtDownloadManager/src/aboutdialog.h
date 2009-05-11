#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QWidget>
#include "ui_aboutdialog.h"

class AboutDialog : public QWidget
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialogClass ui;
};

#endif // ABOUTDIALOG_H
