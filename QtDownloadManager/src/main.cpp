#include "mainwindow.h"

#include <QtGui>
#include <QApplication>
#include <QTcpSocket>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QProcess getpid;
    getpid.start("pidof QtDownloadManager");
    getpid.waitForFinished();
    QString stdout = QString(getpid.readAllStandardOutput());
//  qDebug(QDir::homePath().toAscii().data());
    if(stdout.split(" ").size()>=2){
//    	qDebug("another program running");
    	if(argc>=2){
    		QUdpSocket *udpSocket = new QUdpSocket();
    		QByteArray datagram = argv[1];
    		udpSocket->writeDatagram(datagram.data(), datagram.size(),QHostAddress::LocalHost, 45454);
    	}
    	return 0;
    }
    MainWindow w;
    w.show();
    if(argc>=2){
    	w.AddUrlToAddDialog(argv[1]);
    }
    return a.exec();
}

