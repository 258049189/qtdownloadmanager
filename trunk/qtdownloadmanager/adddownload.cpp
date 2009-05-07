#include "adddownload.h"

AddDownload::AddDownload(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.AD_CancelButton,SIGNAL(clicked()),this,SLOT(AD_CancelButtonClicked()));
	connect(ui.AD_SaveDirToolButton,SIGNAL(clicked()),this,SLOT(AD_SaveDirToolButtonClicked()));
}

AddDownload::~AddDownload()
{

}

int AddDownload::AD_SaveDirToolButtonClicked(){
	QString dirpath = QFileDialog::getExistingDirectory(this, tr("Select deirectory to save the file"),"~/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	ui.AD_SaveDir->setText(dirpath);
	return 0;
}

int AddDownload::AD_CancelButtonClicked(){
	close();
	return 0;
}
