#include "server.h"

Server::Server()
{

}

void Server::incomingConnection(qintptr socketDesctiptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDesctiptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected);

    Sockets.push_back(socket);
    qDebug() << Sockets.size();
    int socketD = socket->socketDescriptor();
    QString ipClient = socket->peerAddress().toString();
    QString portClient = QString::number(socket->peerPort());
    map.insert(socketD, QPair<QString, QString>(ipClient, portClient));
    QString str1 = "Client connected";
    printFromClient(str1);
    QString str = "Hello, client!";
    printTE(str);
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << quint64(0) << str;
    out.device()->seek(0);
    out << quint64(Data.size() - sizeof(quint64));
    socket->write(Data);
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();

    QDataStream in(socket);
    if(in.status() == QDataStream::Ok)
    {

        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 8)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            bool isImage;
            in >> isImage;
            qDebug() << isImage;

            if(isImage == false)
            {
                QString str;
                in >> str;
                nextBlockSize = 0;
                printFromClient(str);
                break;
            }
            else
            {
                QPixmap img;
                in >> img;
                ImageDrawer *imgD = new ImageDrawer();
                imgD->setWindowTitle("Image from client");
                imgD->setMinimumSize(1, 1);
                connect(this, &Server::displayImage, imgD, &ImageDrawer::setImage);
                emit displayImage(img);
                imgD->show();
                nextBlockSize = 0;
                QString confirm = "Image received";
                printFromClient(confirm);
                break;
            }
        }
    }
    else
    {
        QString str1 = "DataStream error";
        printTE(str1);
    }
}

void Server::SendAll(QString str)
{
    if(this->isListening() == true)
    {
        if (Sockets.size() > 0)
        {
            SendToClients(str);
        }
        else
        {
            QString str1 = "Error, there are no clients";
            printTE(str1);
        }
    }
    else
    {
        QString str1 = "Error, server isnt running";
        printTE(str1);
    }
}

void Server::stopServer()
{
    this->close();
    int size = Sockets.size();
    for(int i = 0; i < size; i++)
    {
        Sockets[0]->abort();
        qDebug() << "delete" << i;
    }
    Sockets.clear();
    map.clear();
    QString str = "Server is down";
    printTE(str);
}

void Server::slotDisconnected()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    int sd = socket->state();

    for(int i = 0; i < Sockets.size(); i++)
    {
        if(map.contains(Sockets[i]->socketDescriptor()))
        {
            QPair<QString, QString>values = map.value(Sockets[i]->socketDescriptor());
            int socketD = Sockets[i]->socketDescriptor();
            QString ipC = values.first;
            QString portC = values.second;
            discCl.insert(socketD, QPair<QString, QString>(ipC, portC));
        }
    }

    QList<int> keyM = map.keys();
    QList<int> keyD = discCl.keys();
    int disconnectedC;
    discCl.clear();

    for(int i = 0; i < map.size(); i++)
    {
        if(!keyD.contains(keyM[i]))
        {
            disconnectedC = keyM[i];
        }
    }

    QPair<QString, QString>values = map.value(disconnectedC);
    QString Dip = values.first;
    QString Dport = values.second;
    map.remove(disconnectedC);
    keyM.clear();
    keyD.clear();

    QString time_format = "dd.MM.yyyy HH:mm:ss";
    QDateTime cdt = QDateTime::currentDateTime();
    QString dt = cdt.toString(time_format);
    QString str = "Client disconnected";
    QString str1 = dt + " | " + Dip + ":" + Dport + " | " + str;
    emit fromServerToTE(str1);

    QFile logFile("server.log");
    if(logFile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QString out = str1 + "\n";
        QTextStream stream(&logFile);
        stream << out;
        logFile.flush();
        logFile.close();
        if (stream.status() != QTextStream::Ok)
        {
            QString str2 = "Error during file opening";
            emit fromServerToTE(str2);
        }
    }

    for(int i = 0; i < Sockets.size(); i++)
    {
        if(Sockets[i]->state() == sd)
        {
            Sockets.remove(i);
            break;
        }
    }
    socket->deleteLater();
}

void Server::SendToClients(QString str)
{
    if(str.size() != 0)
    {
        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out << quint64(0) << str;
        out.device()->seek(0);
        out << quint64(Data.size() - sizeof(quint64));
        for(int i = 0; i < Sockets.size(); i++)
        {
            Sockets[i]->write(Data);
        }
        printTE(str);
    }
    else
    {
        QString str2 = "Message cannot be empty";
        printTE(str2);
    }
}

void Server::printTE(QString str)
{
    QString time_format = "dd.MM.yyyy HH:mm:ss";
    QDateTime cdt = QDateTime::currentDateTime();
    QString dt = cdt.toString(time_format);
    QString ipAddr = this->serverAddress().toString() + ":";
    QString portS = QString::number(port);
    QString str1 = dt + " | " + ipAddr + portS + " | " + str;
    emit fromServerToTE(str1);

    QFile logFile("server.log");
    if(logFile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QString out = str1 + "\n";
        QTextStream stream(&logFile);
        stream << out;
        logFile.flush();
        logFile.close();
        if (stream.status() != QTextStream::Ok)
        {
            QString str2 = "Error during file opening";
            emit fromServerToTE(str2);
        }
    }
}

void Server::printFromClient(QString str)
{
    QString time_format = "dd.MM.yyyy HH:mm:ss";
    QDateTime cdt = QDateTime::currentDateTime();
    QString dt = cdt.toString(time_format);
    QPair<QString, QString>values = map.value(socket->socketDescriptor());
    QString ipAddr = values.first;
    QString portS = values.second;
    QString str1 = dt + " | " + ipAddr + ":" + portS + " | " + str;
    emit fromServerToTE(str1);

    QFile logFile("server.log");
    if(logFile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QString out = str1 + "\n";
        QTextStream stream(&logFile);
        stream << out;
        logFile.flush();
        logFile.close();
        if (stream.status() != QTextStream::Ok)
        {
            QString str2 = "Error during file opening";
            emit fromServerToTE(str2);
        }
    }
}

void Server::startServer(int s_port)
{
    if(s_port > 65535)
    {
        qDebug() << "too big number";
        QString str1 = "Error, too big number";
        printTE(str1);
        emit serverIsntStarted();
    }
    else if(s_port < 1)
    {
        qDebug() << "port cannot be negative";
        QString str1 = "Error, port cannot be negative";
        printTE(str1);
        emit serverIsntStarted();
    }
    else
    {
        port = s_port;
        if(this->listen(QHostAddress::LocalHost, port))
        {
            qDebug() << "start";
            QString str1 = "Server is runnung";
            printTE(str1);
        }
        else
        {
            qDebug() << "error";
            QString str1 = "Failed to start server";
            printTE(str1);
        }
    }
    nextBlockSize = 0;
}
