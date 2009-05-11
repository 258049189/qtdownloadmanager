#include "adddownload.h"

AddDownload::AddDownload(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	if(ui.AD_PreferredDownloader->currentText()=="wget"){
		ui.AD_NoOfPieces->setValue(1);
		ui.AD_NoOfPieces->setEnabled(false);
	}
	connect(ui.AD_CancelButton,SIGNAL(clicked()),this,SLOT(AD_CancelButtonClicked()));
	connect(ui.AD_SaveDirToolButton,SIGNAL(clicked()),this,SLOT(AD_SaveDirToolButtonClicked()));
	connect(ui.AD_PreferredDownloader,SIGNAL(currentIndexChanged(QString)),this,SLOT(AD_PreferredDownloaderChanged()));
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

int AddDownload::AD_PreferredDownloaderChanged(){
	if(ui.AD_PreferredDownloader->currentText()=="wget"){
		ui.AD_NoOfPieces->setValue(1);
		ui.AD_NoOfPieces->setEnabled(false);
	}else {
		ui.AD_NoOfPieces->setEnabled(true);
	}
	return 0;
}
