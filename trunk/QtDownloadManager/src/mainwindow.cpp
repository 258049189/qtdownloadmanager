#include "mainwindow.h"


SystemTrayIcon::SystemTrayIcon(){
	defaulticon=0;
	blinking = false;
}

SystemTrayIcon::~SystemTrayIcon(){

}

int SystemTrayIcon::setBlink(bool status){
	if(status){
		blinktimer = new QTimer();
		connect(blinktimer,SIGNAL(timeout()),this,SLOT(blinkicon()));
		blinktimer->start(1000);
		blinking = true;
	}else {
		blinktimer->stop();
		blinking = false;
	}
	return 0;
}

int SystemTrayIcon::blinkicon(){
	if(defaulticon==0){
		setIcon(QIcon(":/images/images/icon.png"));
		defaulticon=1;
	}else{
		setIcon(QIcon(":/images/images/blinkicon.png"));
		defaulticon=0;
	}
	return 0;
}


















MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	InitializeApplication();
}

MainWindow::~MainWindow()
{

}

int MainWindow::InitializeApplication(){
	CheckForConfigFiles();
	ResetVariables();
	GetDefaultVariables();
	SetDownloadsViewModel();
	EstablishConnections();
	LoadPreviousState();
	StartUpdateProgressBar();
	CreateSystemTrayIcon();
	StartListeningForAddUrl();
	return 0;
}

int MainWindow::CheckForConfigFiles(){
	if(!QDir(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager")).exists()){
		QDir().mkdir(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager"));
	}
	if(!QFile::exists(QString("%1/.QtDownloadManager/defaults").arg(QDir::homePath()))){
		QFile::copy(QString("%1/configs/defaults").arg(QApplication::applicationDirPath()),QString("%1/.QtDownloadManager/defaults").arg(QDir::homePath()));
	}
	if(!QDir(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/downloads")).exists()){
		QDir().mkdir(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/downloads"));
	}
	return 0;
}

int MainWindow::ResetVariables(){
	default_downloader = "";
	default_noofpieces = "";
	default_httpproxy = "";
	default_ftpproxy = "";
	default_savedir = "";
	default_downloadsview = "";
	NoOfDownloads=0;
	rowsremoved=0;
	return 0;
}

int MainWindow::GetDefaultVariables(){
	QFile defaults(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/defaults"));
	if (!defaults.open(QIODevice::ReadOnly | QIODevice::Text))
		return 0;
	QTextStream in(&defaults);
//	qDebug("Defaults from file");
//	qDebug("____________________________________________________________________");
	while(!in.atEnd()){
		QString line = in.readLine();
		if(line.contains("downloader:")){
			default_downloader = line.remove("downloader:");
		}else if(line.contains("noofpieces:")){
			default_noofpieces = line.remove("noofpieces:");
		}else if(line.contains("httpproxy:")){
			default_httpproxy = line.remove("httpproxy:");
		}else if(line.contains("ftpproxy:")){
			default_ftpproxy = line.remove("ftpproxy:");
		}else if(line.contains("savedir:")){
			default_savedir = line.remove("savedir:");
		}else if(line.contains("downloadsview:")){
			default_downloadsview = line.remove("downloadsview:");
		}
//		qDebug(line.toAscii().data());
	}
//	qDebug("____________________________________________________________________");
	defaults.close();
	return 0;
}

int MainWindow::SetDownloadsViewModel(){
	ui.FiltersView->expandAll();
//	qDebug(QString("Debug:%1 -> %2").arg("size").arg(default_downloadsview).toAscii().data());
	QStringList DownloadsViewHeaderList = default_downloadsview.split(";");
	model = new QStandardItemModel(0,DownloadsViewHeaderList.size());
	for(int i=0;i<DownloadsViewHeaderList.size();i++){
		model->setHeaderData(i, Qt::Horizontal, QObject::tr(DownloadsViewHeaderList.at(i).toAscii().data()));
	}
	proxyModel = new QSortFilterProxyModel();
	proxyModel->setDynamicSortFilter(true);
	ui.DownloadsList->setModel(proxyModel);
	ui.DownloadsList->setSortingEnabled(true);
	proxyModel->setSourceModel(model);
	selectionModel = ui.DownloadsList->selectionModel();
	ui.DownloadsList->setRootIsDecorated(false);
	ui.DownloadsList->setAlternatingRowColors(true);
	connect(selectionModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(selectionModelCurrentRowChanged(QModelIndex,QModelIndex)));

	ui.DownloadsList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.DownloadsList, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(showContextMenu()));

	return 0;
}

int MainWindow::EstablishConnections(){
	connect(ui.AddButton,SIGNAL(clicked()),this,SLOT(AddButtonClicked()));
	connect(ui.StartButton,SIGNAL(clicked()),this,SLOT(StartButtonClicked()));
	connect(ui.StopButton,SIGNAL(clicked()),this,SLOT(StopButtonClicked()));
	connect(ui.RemoveButton,SIGNAL(clicked()),this,SLOT(RemoveButtonClicked()));
	connect(ui.RevealButton,SIGNAL(clicked()),this,SLOT(RevealButtonClicked()));
	connect(ui.FiltersView,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(FiltersViewcurrentItemChanged()));
	connect(ui.SearchDownloadList,SIGNAL(textChanged(QString)),this,SLOT(SearchDownloadListtextChanged()));
	connect(ui.actionAbout,SIGNAL(activated()),this,SLOT(actionAboutActivated()));
	connect(ui.actionPreferences,SIGNAL(activated()),this,SLOT(actionPreferencesActivated()));
	connect(ui.actionAdd,SIGNAL(activated()),ui.AddButton,SLOT(animateClick()));
	connect(ui.actionReveal,SIGNAL(activated()),ui.RevealButton,SLOT(animateClick()));
	connect(ui.actionStart,SIGNAL(activated()),ui.StartButton,SLOT(animateClick()));
	connect(ui.actionStop,SIGNAL(activated()),ui.StopButton,SLOT(animateClick()));
	connect(ui.actionEdit,SIGNAL(activated()),ui.EditButton,SLOT(animateClick()));
	connect(ui.actionRemove,SIGNAL(activated()),ui.RemoveButton,SLOT(animateClick()));
	connect(ui.actionExit,SIGNAL(activated()),qApp,SLOT(quit()));
	connect(ui.actionFullScreen,SIGNAL(activated()),this,SLOT(showFullScreen()));
	connect(ui.actionNormal,SIGNAL(activated()),this,SLOT(showNormal()));

	return 0;
}


int MainWindow::LoadPreviousState(){

	QStringList files=QDir(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/downloads/")).entryList();
	for(int i=0;i<files.size();i++){
		if(files.at(i)!="."&&files.at(i)!=".."&&!files.at(i).endsWith("~")){
//			qDebug(QString("%1:%2").arg(i).arg(files.at(i)).toAscii().data());
			AddDownloadToListAndProcess(QString("%1%2").arg(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/downloads/")).arg(files.at(i)));
		}
	}
	return 0;
}


int MainWindow::AddButtonClicked(){
//	qDebug("Add Button Clicked");
	adddownloaddialog = new AddDownload();
	adddownloaddialog->show();

	adddownloaddialog->ui.AD_NoOfPieces->setValue(default_noofpieces.toInt());
	int default_downloader_index=0;
	default_downloader_index=adddownloaddialog->ui.AD_PreferredDownloader->findText(default_downloader);
	adddownloaddialog->ui.AD_PreferredDownloader->setCurrentIndex(default_downloader_index);
	adddownloaddialog->ui.AD_SaveDir->setText(default_savedir);

	connect(adddownloaddialog->ui.AD_AddButton,SIGNAL(clicked()),this,SLOT(ADDialog_AddButtonClicked()));

	return 0;
}

int MainWindow::ADDialog_AddButtonClicked(){
	QDate *dateadded = new QDate();

	QString configfilepath;
	QString urlfilepath = adddownloaddialog->ui.AD_Url->text();
	urlfilepath.remove("/");
	if(urlfilepath.size()>100){
		urlfilepath.resize(100);
	}
	configfilepath=QString("%1%2.%3").arg(QString("%1/%2").arg(QDir::homePath()).arg(".QtDownloadManager/downloads/")).arg(urlfilepath).arg(adddownloaddialog->ui.AD_PreferredDownloader->currentText());
//	qDebug(QString("%1").arg(adddownloaddialog->ui.AD_Url->text().size()).toAscii().data());
	QFile configfile(configfilepath);
	if (!configfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return 0;

	QTextStream out(&configfile);
	out << "Download:" <<endl;
	if(adddownloaddialog->ui.AD_StartNow->isChecked())
		out << "Status:" << "0%(running)"<<endl;
	else
		out << "Status:" << "0%(added)"<<endl;
	out << "TotalSize:"<<endl;
	out << "Speed:"<<endl;
	out << "Downloader:" << adddownloaddialog->ui.AD_PreferredDownloader->currentText()<<endl;
	out <<"Pieces:"<<adddownloaddialog->ui.AD_NoOfPieces->value()<<endl;
	out <<"Url:"<<adddownloaddialog->ui.AD_Url->text()<<endl;
	out <<"SaveDir:"<<adddownloaddialog->ui.AD_SaveDir->text()<<endl;
	out << "ElapsedTime:" <<endl;
	out << "EstimatedTime:" <<endl;
	out << "DateAdded:" << dateadded->currentDate().toString("dd/MM/yyyy")<< endl;

	configfile.close();


	adddownloaddialog->~AddDownload();
	AddDownloadToListAndProcess(configfilepath);

	return 0;
}

int MainWindow::AddDownloadToListAndProcess(QString filepath){
	int column=0;
	int startimediately =0;
	model->insertRow(NoOfDownloads);
	QFile configfile(filepath);
	if (!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
			return 0;
	QTextStream in(&configfile);
	QStringList DownloadsViewHeaderList = default_downloadsview.split(";");

	while (!in.atEnd()) {
		QString line = in.readLine();
		for(int i=0;i<DownloadsViewHeaderList.size();i++){
			if(line.startsWith(QString("%1:").arg(DownloadsViewHeaderList.at(i)))){
				if(line.startsWith("Status:")){
					line.remove(QString("%1:").arg(DownloadsViewHeaderList.at(i)));
					QStandardItem *item = new QStandardItem(line);
					if(line.contains("(added)")){
						item->setBackground(QColor("#ffe0e0"));
						item->setForeground(QColor("#000000"));
					}else if(line.contains("(paused)")){
						item->setBackground(QColor("#ffe070"));
						item->setForeground(QColor("#000000"));
					}else if(line.contains("(running)")){
						startimediately =1;
						item->setBackground(QColor("#6ee070"));
						item->setForeground(QColor("#000000"));
					}else if(line.contains("(error)")){
						item->setBackground(QColor("#ff5151"));
						item->setForeground(QColor("#000000"));
					}else if(line.contains("(completed)")){
						item->setBackground(QColor("#52c0ff"));
						item->setForeground(QColor("#000000"));
					}
					model->setItem(NoOfDownloads, column, item);
				}else{
					line.remove(QString("%1:").arg(DownloadsViewHeaderList.at(i)));
					model->setData(model->index(NoOfDownloads,column),line);
				}
			}
		}
		column++;
	}

	downloads[NoOfDownloads] = new Downloads();
	downloads[NoOfDownloads]->SetupDownloadData(model,NoOfDownloads,filepath);
	if(startimediately==1)
		downloads[NoOfDownloads]->StartDownload();
	connect(downloads[NoOfDownloads],SIGNAL(completed(QString)),this,SLOT(ADownloadCompleted(QString)));
	connect(downloads[NoOfDownloads],SIGNAL(updateinterface()),this,SLOT(UpdateInterface()));

	configfile.close();
	NoOfDownloads++;
	return 0;
}

int MainWindow::StartButtonClicked(){
//	qDebug("Start Button Clicked");
	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = 0;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}
//	qDebug(QString("%1").arg(row).toAscii().data());

	if(downloads[row]->DL_Status.contains("(completed)")){
		QMessageBox mb(QMessageBox::Question,tr("Please choose"),tr("Check if u want to remove the previously downloaded file or the downloader will alot a new name"),QMessageBox::Ok | QMessageBox::Cancel,this);
		QCheckBox * mbCheckBox = new QCheckBox("delete the downloaded files",this);
		mb.layout()->addWidget(mbCheckBox);
		if(mb.exec() != QMessageBox::Ok) {
			return 0;
		}
		if(mbCheckBox->isChecked()){
			QString filename = QString("%1/%2").arg(downloads[row]->DL_SaveDir).arg(downloads[row]->DL_Download);
			QFile::remove(filename);
		}
	}

	downloads[row]->StartDownload();
	return 0;
}

int MainWindow::StopButtonClicked(){
//	qDebug("Stop Button Clicked");
	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = -1;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}
//	qDebug(QString("%1").arg(row).toAscii().data());
	if(row>=0){
		downloads[row]->StopDownload();
	}
	return 0;
}

int MainWindow::StartUpdateProgressBar(){
	QTimer *updatetimer = new QTimer();
	connect(updatetimer, SIGNAL(timeout()), this, SLOT(UpdateProgressBar()));
	updatetimer->start(1000);
	return 0;
}

int MainWindow::UpdateProgressBar(){
	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = -1;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}
	if(row!=-1){
		QString value = model->data(model->index(row,1)).toString().remove("(running)").remove("(paused)").remove("(added)").remove("(completed)").remove("%");
		ui.DownloadProgressBar->setValue(value.toInt());
	}
	return 0;
}

int MainWindow::RemoveButtonClicked(){
	QMessageBox mb(QMessageBox::Warning,tr("Please Confirm"),tr("Do you want to remove the Download Entry"),QMessageBox::Ok | QMessageBox::Cancel,this);
	QCheckBox * mbCheckBox = new QCheckBox("delete the downloaded files",this);
	mb.layout()->addWidget(mbCheckBox);
	if(mb.exec() != QMessageBox::Ok) {

		return 0;
	}


	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = -1;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}
	downloads[row]->StopDownload();

	if(mbCheckBox->isChecked()){
		QString filename = QString("%1/%2").arg(downloads[row]->DL_SaveDir).arg(downloads[row]->DL_Download);
		QFile::remove(filename);
		filename = QString("%1/%2.st").arg(downloads[row]->DL_SaveDir).arg(downloads[row]->DL_Download);
		QFile::remove(filename);
	}

	QFile::remove(downloads[row]->localfilepath);
	model->removeRow(row);
	for(int i=0;i<NoOfDownloads;i++){
		if(i>row){
			downloads[i]->localrow = downloads[i]->localrow -1 ;
			downloads[i-1]=downloads[i];
		}
	}
	NoOfDownloads--;
	rowsremoved++;
	return 0;
}

int MainWindow::RevealButtonClicked(){
	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = -1;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}
	if(row!=-1){
		QProcess::execute(QString("nautilus %1").arg(downloads[row]->DL_SaveDir));
	}
	return 0;
}

int MainWindow::FiltersViewcurrentItemChanged(){
	QString filter=ui.FiltersView->currentItem()->text(0);
	if(filter!="Downloads"){
		proxyModel->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive,QRegExp::FixedString));
		proxyModel->setFilterKeyColumn(1);
	}else{
		proxyModel->setFilterRegExp(QRegExp("", Qt::CaseInsensitive,QRegExp::FixedString));
		proxyModel->setFilterKeyColumn(1);
	}
	return 0;
}

int MainWindow::SearchDownloadListtextChanged(){
	proxyModel->setFilterRegExp(QRegExp(ui.SearchDownloadList->text(), Qt::CaseInsensitive,QRegExp::FixedString));
	proxyModel->setFilterKeyColumn(0);
	return 0;
}

int MainWindow::CreateSystemTrayIcon(){
	QAction *ShowHideAction = new QAction("show/hide Interface",this);
	connect(ShowHideAction, SIGNAL(triggered()), this, SLOT(ShowHideInterface()));
	QAction *QuitAction = new QAction("Quit",this);
	connect(QuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	TrayIconMenu = new QMenu();
	TrayIconMenu->addAction(ShowHideAction);
	TrayIconMenu->addSeparator();
	TrayIconMenu->addAction(QuitAction);

	TrayIcon = new SystemTrayIcon();
	TrayIcon->setIcon(QIcon(":/images/images/icon.png"));
	TrayIcon->setContextMenu(TrayIconMenu);
	TrayIcon->setVisible(true);
	TrayIcon->setToolTip("QtDownloadManager\n -Gui frontend for console download accelerators");
	connect(TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason)));

	return 0;
}

int MainWindow::StartListeningForAddUrl(){
	udpSocket = new QUdpSocket();
	udpSocket->bind(QHostAddress::LocalHost,45454);
	connect(udpSocket, SIGNAL(readyRead()),this, SLOT(ReadUrlAndProcess()));
	return 0;
}

int MainWindow::ReadUrlAndProcess(){
	QByteArray datagram;
	while(udpSocket->hasPendingDatagrams()){
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
//      qDebug(QString(datagram.data()).toAscii().data());
	}
	AddUrlToAddDialog(QString(datagram.data()));
	return 0;
}


int MainWindow::AddUrlToAddDialog(QString Url){
	adddownloaddialog = new AddDownload();
	adddownloaddialog->show();

	adddownloaddialog->ui.AD_NoOfPieces->setValue(default_noofpieces.toInt());
	int default_downloader_index=0;
	default_downloader_index=adddownloaddialog->ui.AD_PreferredDownloader->findText(default_downloader);
	adddownloaddialog->ui.AD_PreferredDownloader->setCurrentIndex(default_downloader_index);
	adddownloaddialog->ui.AD_SaveDir->setText(default_savedir);
	adddownloaddialog->ui.AD_Url->setText(Url);
	connect(adddownloaddialog->ui.AD_AddButton,SIGNAL(clicked()),this,SLOT(ADDialog_AddButtonClicked()));

	return 0;
}



int MainWindow::ShowHideInterface(){
	if(isHidden()){
		show();
	}else{
		hide();
	}
	return 0;
}

int MainWindow::TrayIconActivated(QSystemTrayIcon::ActivationReason reason){
	switch (reason){
		case QSystemTrayIcon::Trigger:
			if(isHidden()){
				if(TrayIcon->blinking){
					TrayIcon->setBlink(false);
					TrayIcon->setIcon(QIcon(":/images/images/icon.png"));
				}
				show();
			}else{
				hide();
			}
			break;
		case QSystemTrayIcon::DoubleClick:
			break;
		case QSystemTrayIcon::MiddleClick:
			break;
		default:
			;
	}
	return 0;
}



int MainWindow::selectionModelCurrentRowChanged(QModelIndex current,QModelIndex previous){
//	qDebug("updating interface");
	int row = -1;
	row = proxyModel->mapToSource(current).row();

//	qDebug(QString("%1").arg(row).toAscii().data());
	QString value = model->data(model->index(row,1)).toString();
	if(value.contains("running")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(false);
		ui.StopButton->setEnabled(true);
		ui.EditButton->setEnabled(false);
	}else if(value.contains("paused")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("error")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("completed")){
		ui.StartButton->setText("Restart");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("added")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}

//	UpdateInterface();
	return 0;
}

int MainWindow::UpdateInterface(){
//	qDebug("updating interface");
	QModelIndexList indexes = selectionModel->selectedRows();
	QModelIndex index;
	int row = -1;
	foreach (index, indexes) {
		row = proxyModel->mapToSource(index).row();
	}

//	qDebug(QString("%1").arg(row).toAscii().data());

	QString value = model->data(model->index(row,1)).toString();
	if(value.contains("running")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(false);
		ui.StopButton->setEnabled(true);
		ui.EditButton->setEnabled(false);
	}else if(value.contains("paused")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("error")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("completed")){
		ui.StartButton->setText("Restart");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}else if(value.contains("added")){
		ui.StartButton->setText("Start");
		ui.StartButton->setEnabled(true);
		ui.StopButton->setEnabled(false);
		ui.EditButton->setEnabled(true);
	}
//	qDebug(QString("%1").arg(row).toAscii().data());
	return 0;
}

int MainWindow::ADownloadCompleted(QString message){
	TrayIcon->showMessage("Download Completed",message,QSystemTrayIcon::Information,5000);
	if(isHidden()){
		if(!TrayIcon->blinking)
			TrayIcon->setBlink(true);
	}
	return 0;
}


int MainWindow::actionAboutActivated(){
	AboutDialog *dialog = new AboutDialog();
	dialog->show();
	return 0;
}

int MainWindow::actionPreferencesActivated(){
	PreferencesDialog *dialog = new PreferencesDialog();
	dialog->show();

	return 0;
}

int MainWindow::showContextMenu(){
	QMenu *menu = new QMenu();
	QPixmap startpix(":/images/images/buttons/StartButton.png");
	QPixmap stoppix(":/images/images/buttons/StopButton.png");
	QPixmap editpix(":/images/images/buttons/EditButton.png");
	QPixmap revealpix(":/images/images/buttons/RevealButton.png");
	QPixmap removepix(":/images/images/buttons/RemoveButton.png");

	QAction *start = new QAction(startpix, "Start", this);
	QAction *stop = new QAction(stoppix, "Stop", this);
	QAction *edit = new QAction(editpix, "Edit", this);
	QAction *reveal = new QAction(revealpix, "Reveal", this);
	QAction *remove = new QAction(removepix, "Remove", this);
	QAction *properties = new QAction("Properties", this);

	menu->addAction(start);
	menu->addAction(stop);
	menu->addAction(edit);
	menu->addAction(reveal);
	menu->addAction(remove);
	menu->addSeparator();
	menu->addAction(properties);
	QString selected = menu->exec(QCursor::pos())->text();
//	qDebug(selected.toAscii().data());
	if(selected == "Start"){
		ui.StartButton->animateClick();
	}else if(selected == "Stop"){
		ui.StopButton->animateClick();
	}else if(selected == "Edit"){
		ui.EditButton->animateClick();
	}else if(selected == "Reveal"){
		ui.RevealButton->animateClick();
	}else if(selected == "Remove"){
		ui.RemoveButton->animateClick();
	}
	return 0;
}
