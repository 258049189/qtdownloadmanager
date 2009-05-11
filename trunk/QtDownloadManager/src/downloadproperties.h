#ifndef DOWNLOADPROPERTIES_H
#define DOWNLOADPROPERTIES_H

#include <QtGui/QWidget>
#include "ui_downloadproperties.h"

class DownloadProperties : public QWidget
{
    Q_OBJECT

public:
    DownloadProperties(QWidget *parent = 0);
    ~DownloadProperties();

private:
    Ui::DownloadPropertiesClass ui;
};

#endif // DOWNLOADPROPERTIES_H
