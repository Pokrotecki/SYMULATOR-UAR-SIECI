#include "Client.h"
#include <QDebug>

Client::Client(const QString& host, quint16 port, QObject* parent)
    : QObject(parent)
{
    connect(&socket, &QTcpSocket::readyRead,
            this, &Client::onReadyRead);

    connect(&socket, &QTcpSocket::connected,
            this, [this]()
            {
                qDebug() << "KLIENT: Połączono z serwerem!";
                emit connectedOk();
            });

    connect(&socket, &QTcpSocket::disconnected,
            this, [this]()
            {
                qDebug() << "KLIENT: rozłączono z serwerem";
                emit disconnected();
            });

    connect(&socket, &QTcpSocket::errorOccurred,
            [](QAbstractSocket::SocketError err)
            {
                qDebug() << "KLIENT: Błąd połączenia:" << err;
            });

    socket.connectToHost(host, port);

    socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

void Client::sendConfig(const ConfigPacket& c)
{
    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);

    out << quint32(0);   // placeholder
    out << quint16(2);   // typ = ConfigPacket
    out << c;

    out.device()->seek(0);
    out << quint32(buf.size() - sizeof(quint32));

    socket.write(buf);
}

void Client::onReadyRead()
{
    buffer.append(socket.readAll());

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

        if (type == 1) {
            StepPacket p;
            in >> p;
            emit stepReceived(p);
        }
        if (type == 4)
        {
            OutputPacket p;
            in >> p;

            emit outputReceived(p.seq, p.y);
        }
    }
}

//IDK
void Client::sendControl(quint32 seq, double u, double w)
{
    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);


    ControlPacket p;
    p.seq = seq;
    p.u = u;
    p.w = w;

    out << quint32(0);
    out << quint16(3);
    out << p;

    out.device()->seek(0);
    out << quint32(buf.size() - sizeof(quint32));

    socket.write(buf);
}
