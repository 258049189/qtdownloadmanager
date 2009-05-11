/*
 * Downloads.h
 *
 *  Created on: 25 Apr, 2009
 *      Author: vijay
 */

#ifndef DOWNLOADS_H_
#define DOWNLOADS_H_

#include <QObject>
#include <QProcess>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QApplication>

class Downloads : public QObject {

	Q_OBJECT

public:
	Downloads();
	~Downloads();
	int StartDownload();
	int StopDownload();
	int SetupDownloadData(QStandardItemModel *,int,QString);
	int localrow;
	QString localfilepath;
	QString DL_SaveDir;
	QString DL_Download;
	QString DL_Status;
private:
	QProcess *proc;
	QStandardItemModel *localmodel;

	QString DL_TotalSize;
	QString DL_Speed;
	QString DL_Downloader;
	QString DL_Pieces;
	QString DL_Url;
	QString DL_ElapsedTime;
	QString DL_EstimatedTime;

	int DownloadRunning;
	int ReadParameterToDownload();
	QTimer *updatetimer;

public slots:
	int SaveDownloadState();
	int ReadFromStdoutAndProcess();

signals:
	void completed(QString);
	void updateinterface();
};

#endif /* DOWNLOADS_H_ */
