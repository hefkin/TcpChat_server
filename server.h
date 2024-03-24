#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QDateTime>
#include <QFile>
#include <QMap>
#include <QPair>
#include <QList>
#include <QLabel>
#include "imagedrawer.h"

class Server:public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    int port = 25;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    quint64 nextBlockSize;
    void SendToClients(QString str);
    void printTE(QString str);
    void printFromClient(QString str);
    QMap<int, QPair<QString, QString>> map;
    QMap<int, QPair<QString, QString>> discCl;

public slots:
    void startServer(int s_port);
    void slotReadyRead();
    void SendAll(QString str);
    void stopServer();
    void slotDisconnected();

protected slots:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void fromServerToTE(QString);
    void serverIsntStarted();
    void displayImage(QPixmap);

};

#endif // SERVER_H
