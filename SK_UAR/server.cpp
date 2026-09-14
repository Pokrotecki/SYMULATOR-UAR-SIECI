#include "Server.h"



#include <QDebug>

Server::Server(quint16 port, QObject* parent)
    : QObject(parent)
{
    connect(&server, &QTcpServer::newConnection,
            this, &Server::onNewConnection);

    server.listen(QHostAddress::Any, port);
}

void Server::onNewConnection()
{
    socket = server.nextPendingConnection();

    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    connect(socket, &QTcpSocket::readyRead,
            this, &Server::onReadyRead);

    connect(socket, &QTcpSocket::disconnected,
            this, [this]()
            {
                qDebug() << "SERWER: klient rozłączony";

                socket->deleteLater();
                socket = nullptr;

                emit disconnected();
            });

    emit connectedOk();
}

void Server::sendStep(const StepPacket& p)
{
    if (!socket) return;

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);

    out << quint32(0);
    out << quint16(1);   // typ = StepPacket
    out << p;

    out.device()->seek(0);
    out << quint32(buf.size() - sizeof(quint32));

    socket->write(buf);
}

//IDK
void Server::sendOutput(const OutputPacket& p)
{
    if (!socket) return;

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);

    out << quint32(0);
    out << quint16(4);
    out << p;

    out.device()->seek(0);
    out << quint32(buf.size() - sizeof(quint32));

    socket->write(buf);
}

// pozwala obiektowi odeslac regulatorowi swoja aktualna konfiguracje na potrzeby symulacji w tle
void Server::sendConfig(const ConfigPacket& c)
{
    if (!socket) return;

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);

    out << quint32(0);
    out << quint16(2);   // typ = ConfigPacket
    out << c;

    out.device()->seek(0);
    out << quint32(buf.size() - sizeof(quint32));

    socket->write(buf);
}

void Server::onReadyRead()
{
    buffer.append(socket->readAll());

    while (true)
    {
        if (buffer.size() < 4)
            return;

        QDataStream sizeStream(buffer);
        quint32 size;
        sizeStream >> size;

        if (buffer.size() < 4 + size)
            return;

        QByteArray packet = buffer.mid(4, size);
        buffer.remove(0, 4 + size);

        QDataStream in(packet);
        quint16 type;
        in >> type;

        if (type == 2) {
            ConfigPacket c;
            in >> c;
            emit configReceived(c);
        }
        if (type == 3) // IDK
        {
            ControlPacket p;
            in >> p;

            //qDebug() << "SERWER: odebrano u =" << p.u;

            emit sterowanieReceived(p.seq, p.u, p.w);
        }
    }
}
