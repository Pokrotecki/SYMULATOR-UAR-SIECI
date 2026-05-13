#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"
#include "ConfigPacket.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString& host, quint16 port, QObject* parent = nullptr);

    void sendConfig(const ConfigPacket& c);

signals:
    void connectedOk();
    void stepReceived(const StepPacket& p);

private slots:
    void onReadyRead();

private:
    QTcpSocket socket;
    QByteArray buffer;
};

#endif // CLIENT_H
