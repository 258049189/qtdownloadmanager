#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QWidget>
#include "ui_preferencesdialog.h"

class PreferencesDialog : public QWidget
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private:
    Ui::PreferencesDialogClass ui;
};

#endif // PREFERENCESDIALOG_H
