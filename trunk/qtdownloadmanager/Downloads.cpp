/*
 * Downloads.cpp
 *
 *  Created on: 25 Apr, 2009
 *      Author: vijay
 */

#include "Downloads.h"

Downloads::Downloads() {
	// TODO Auto-generated constructor stub

}

Downloads::~Downloads() {
	// TODO Auto-generated destructor stub
}

int Downloads::StartDownload(){
	DownloadRunning = 1;
//	ReadParameterToDownload();
	DL_Speed = "0kb/s";
	localmodel->setData(localmodel->index(localrow,3),DL_Speed);
	if(DL_TotalSize.contains("Undetermined")){
		QString filename = QString("%1/%2").arg(DL_SaveDir).arg(DL_Download);
		QFile::remove(filename);
	}

	DL_Status = localmodel->data(localmodel->index(localrow,1)).toString();
	if(DL_Status.contains("(added)")){
		DL_Status.replace("(added)","(running)");
	}else if(DL_Status.contains("(paused)")){
		DL_Status.replace("(paused)","(running)");
	}else if(DL_Status.contains("(error)")){
		DL_Status.replace("(error)","(running)");
	}else if(DL_Status.contains("(completed)")){
		DL_Status = "0%(running)";
	}
	SaveDownloadState();
	QStandardItem *item = new QStandardItem(DL_Status);
	item->setBackground(QColor("#6ee070"));
	item->setForeground(QColor("#000000"));
	localmodel->setItem(localrow, 1, item);
	emit updateinterface();
	if(DL_Downloader.contains("axel")){
		proc = new QProcess();
		QString command = "axel";
		QStringList arguments;
		arguments <<"-n"<< DL_Pieces << DL_Url;
		proc->setWorkingDirectory(DL_SaveDir);
		proc->start(command,arguments);
		connect( proc, SIGNAL( readyReadStandardOutput()), this, SLOT(ReadFromStdoutAndProcess()) );

	}

	updatetimer = new QTimer();
	connect(updatetimer, SIGNAL(timeout()), this, SLOT(SaveDownloadState()));
	updatetimer->start(5000);

	return 0;
}


int Downloads::SetupDownloadData(QStandardItemModel *model,int row,QString filepath){
	localmodel = model;
	localrow = row;
	localfilepath = filepath;
	ReadParameterToDownload();
	return 0;
}


int Downloads::ReadParameterToDownload(){
	QFile configfile(localfilepath);
	if (!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
		return 0;
	QTextStream in(&configfile);

	while(!in.atEnd()){
		QString line = in.readLine();
		if(line.startsWith("Downloader:")){
			DL_Downloader = line.remove("Downloader:");
		}else if(line.startsWith("Pieces:")){
			DL_Pieces = line.remove("Pieces:");
		}else if(line.startsWith("Url:")){
			DL_Url = line.remove("Url:");
		}else if(line.startsWith("SaveDir:")){
			DL_SaveDir = line.remove("SaveDir:");
		}else if(line.startsWith("Speed:")){
			DL_Speed = line.remove("Speed:");
		}else if(line.startsWith("Status:")){
			DL_Status = line.remove("Status:");
		}else if(line.startsWith("Download:")){
			DL_Download = line.remove("Download:");
		}else if(line.startsWith("TotalSize:")){
			DL_TotalSize = line.remove("TotalSize:");
		}else if(line.startsWith("ElapsedTime:")){
			DL_ElapsedTime = line.remove("ElapsedTime:");
		}else if(line.startsWith("EstimatedTime:")){
			DL_EstimatedTime = line.remove("EstimatedTime:");
		}
	}

	configfile.close();
	return 0;
}




int Downloads::SaveDownloadState(){
//	qDebug("saving state of the download");
//	qDebug("________________________________________________________________");
	QString data="";
	QFile configfile(localfilepath);
	if(!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
		return 0;
	QTextStream in(&configfile);
	while(!in.atEnd()){
		QString line = in.readLine();
		if(line.startsWith("Download:")){
			line = QString("Download:%1").arg(DL_Download);
		}else if(line.startsWith("Status:")){
			line = QString("Status:%1").arg(DL_Status);
		}else if(line.startsWith("TotalSize:")){
			line = QString("TotalSize:%1").arg(DL_TotalSize);
		}else if(line.startsWith("Speed:")){
			line = QString("Speed:%1").arg(DL_Speed);
		}else if(line.startsWith("Downloader:")){
			line = QString("Downloader:%1").arg(DL_Downloader);
		}else if(line.startsWith("Pieces:")){
			line = QString("Pieces:%1").arg(DL_Pieces);
		}else if(line.startsWith("Url:")){
			line = QString("Url:%1").arg(DL_Url);
		}else if(line.startsWith("SaveDir:")){
			line = QString("SaveDir:%1").arg(DL_SaveDir);
		}else if(line.startsWith("ElapsedTime:")){
			line = QString("ElapsedTime:%1").arg(DL_ElapsedTime);
		}else if(line.startsWith("EstimatedTime:")){
			line = QString("EstimatedTime:%1").arg(DL_EstimatedTime);
		}
		data.append(line).append("\n");
	}
//	qDebug(data.toAscii().data());
	configfile.close();

	QFile updatedconfigfile(localfilepath);
	if (!updatedconfigfile.open(QIODevice::WriteOnly | QIODevice::Text))
		return 0;
	QTextStream out(&updatedconfigfile);
	out << data;

	updatedconfigfile.close();
	return 0;
}


int Downloads::ReadFromStdoutAndProcess(){
	while(proc->canReadLine()) {
		QString line = proc->readLine();
//		qDebug(line.toAscii().data());
		if(DL_Downloader == "axel"){
			QStringList linesplitlist;
			line.remove("\n");
			if(line.contains("File size:")){
				DL_TotalSize = line.remove("File size:").remove(" ").remove("bytes");
				localmodel->setData(localmodel->index(localrow,2),DL_TotalSize);
			}else if(line.contains("Opening output file ")){
				DL_Download = line.remove("Opening output file ");
				localmodel->setData(localmodel->index(localrow,0),DL_Download);
			}else if (line.contains("%")){
//				qDebug(line.toAscii().data());
//				line.remove("..").remove(",").remove(" ").remove("[");
				line.remove(" ").replace("[","]");
//				qDebug(QString("%1").arg(line).toAscii().data());
				linesplitlist = line.split("]",QString::SkipEmptyParts);
				if(linesplitlist.size()>=3){
					DL_Status = QString("%1(running)").arg(linesplitlist.at(0));
					QStandardItem *item = new QStandardItem(DL_Status);
					item->setBackground(QColor("#6ee070"));
					item->setForeground(QColor("#000000"));
					localmodel->setItem(localrow, 1, item);
//					emit updateinterface();
					DL_Speed = linesplitlist.at(2);
					localmodel->setData(localmodel->index(localrow,3),DL_Speed);
				}
			}else if(line.startsWith("Downloaded") && line.contains("in")){
				DL_Status = QString("100%(completed)");
				QStandardItem *item = new QStandardItem(DL_Status);
				item->setBackground(QColor("#52c0ff"));
				item->setForeground(QColor("#000000"));
				localmodel->setItem(localrow, 1, item);
				emit updateinterface();
				updatetimer->stop();
				SaveDownloadState();
				QString completedstring ="";
				completedstring = QString("FileName:%1\nUrl:%2\nSaveDir:%3").arg(DL_Download).arg(DL_Url).arg(DL_SaveDir);
				emit completed(completedstring);
			}else if(line.startsWith("Connection gone.") || (line.startsWith("HTTP/1.0")  && ( line.contains("Time-out") || line.contains("Unavailable") || line.contains("Forbidden")))){
				DL_Status = localmodel->data(localmodel->index(localrow,1)).toString();
				if(DL_Status.contains("(added)")){
					DL_Status.replace("(added)","(error)");
				}else if(DL_Status.contains("(paused)")){
					DL_Status.replace("(paused)","(error)");
				}else if(DL_Status.contains("(running)")){
					DL_Status.replace("(running)","(error)");
				}
				QStandardItem *item = new QStandardItem(DL_Status);
				item->setBackground(QColor("#ff5151"));
				item->setForeground(QColor("#000000"));
				localmodel->setItem(localrow, 1, item);
				emit updateinterface();
				SaveDownloadState();
			}else if(line.contains("Server unsupported")){
				DL_TotalSize = "Undetermined";
				localmodel->setData(localmodel->index(localrow,2),DL_TotalSize);
			}
		}
	}
//	SaveDownloadState();
	return 0;
}

int Downloads::StopDownload(){
	if(DownloadRunning!=1)
		return 0;
	proc->close();
	DownloadRunning = 0;

	DL_Status = localmodel->data(localmodel->index(localrow,1)).toString();
	if(DL_Status.contains("(running)")){
		DL_Status.replace("(running)","(paused)");
	}else if(DL_Status.contains("(added)")){
		DL_Status.replace("(added)","(paused)");
	}
	QStandardItem *item = new QStandardItem(DL_Status);
	item->setBackground(QColor("#ffe070"));
	item->setForeground(QColor("#000000"));
	localmodel->setItem(localrow, 1, item);
	emit updateinterface();

	updatetimer->stop();
	SaveDownloadState();

	return 0;
}

