#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QFile>
#include <QTextStream>
#include "adddownload.h"
#include "Downloads.h"
#include <QTimer>
#include <QTreeWidgetItem>
#include <QSystemTrayIcon>
#include "aboutdialog.h"
#include "preferencesdialog.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QMessageBox>
#include <QDir>
#include <QDate>

class SystemTrayIcon : public QSystemTrayIcon{
	Q_OBJECT
public:
	SystemTrayIcon();
	~SystemTrayIcon();
	int setBlink(bool);
	QTimer *blinktimer;
	int defaulticon;
	bool blinking;
public slots:
	int blinkicon();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
	int InitializeApplication();
	QString default_downloader;
	QString default_noofpieces;
	QString default_httpproxy;
	QString default_ftpproxy;
	QString default_savedir;
	QString default_downloadsview;
	int ResetVariables();
	int GetDefaultVariables();
	int SetDownloadsViewModel();
	int EstablishConnections();
	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;
	QItemSelectionModel *selectionModel;
	AddDownload *adddownloaddialog;
	int NoOfDownloads;
	int AddDownloadToListAndProcess(QString);
	Downloads *downloads[100];
	int LoadPreviousState();
	int StartUpdateProgressBar();
	int rowsremoved;
	SystemTrayIcon *TrayIcon;
	int CreateSystemTrayIcon();
	QMenu *TrayIconMenu;
	int StartListeningForAddUrl();
	QUdpSocket *udpSocket ;
	int AddUrlToAddDialog(QString);

public slots:
	int AddButtonClicked();
	int StartButtonClicked();
	int ADDialog_AddButtonClicked();
	int StopButtonClicked();
	int UpdateProgressBar();
	int RemoveButtonClicked();
	int RevealButtonClicked();
	int FiltersViewcurrentItemChanged();
	int SearchDownloadListtextChanged();
	int TrayIconActivated(QSystemTrayIcon::ActivationReason);
	int ShowHideInterface();
	int selectionModelCurrentRowChanged(QModelIndex,QModelIndex);
	int UpdateInterface();
	int ADownloadCompleted(QString);
	int actionAboutActivated();
	int actionPreferencesActivated();
	int ReadUrlAndProcess();

private:
    Ui::MainWindowClass ui;
};



#endif // MAINWINDOW_H
