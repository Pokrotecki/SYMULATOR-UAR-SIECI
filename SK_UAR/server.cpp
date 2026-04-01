#include "Server.h"
#include <QDebug>

Server::Server(quint16 port, QObject* parent)
    : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    server.listen(QHostAddress::Any, port);
    qDebug() << "Serwer nasłuchuje na porcie" << port;
}

void Server::onNewConnection()
{
    socket = server.nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    qDebug() << "Połączono z klientem";
}

void Server::onReadyRead()
{
    QDataStream in(socket);
    StepPacket p;
    in >> p;

    qDebug() << "Serwer odebrał krok:" << p.k << " y=" << p.y;

    StepPacket reply = p;
    reply.y += 1; // przykładowa modyfikacja

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out << reply;

    socket->write(buf);
}
