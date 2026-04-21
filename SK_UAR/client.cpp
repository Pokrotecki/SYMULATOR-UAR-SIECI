#include "Client.h"
#include <QDebug>

Client::Client(const QString& host, quint16 port, QObject* parent)
    : QObject(parent)
{
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(&socket, &QTcpSocket::connected,  /*this,*/ [this]() //czy this powinno oba ?
    {
        qDebug() << "KLIENT: Połączono z serwerem!";
        emit connectedOk();
    });
    connect(&socket, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError err)
    {
        qDebug() << "KLIENT: Błąd połączenia:" << err;
    });
    socket.connectToHost(host, port);
    qDebug() << "Klient łączy się z" << host << port;
}

void Client::sendStep(const StepPacket& p)
{
    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out << p;
    socket.write(buf);
}

void Client::onReadyRead()
{
    QDataStream in(&socket);
    StepPacket p;
    in >> p;

    qDebug() << "Klient odebrał odpowiedź:" << p.k << " y=" << p.y;
}
