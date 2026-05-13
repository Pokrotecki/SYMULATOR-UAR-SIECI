#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"
#include "ConfigPacket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject* parent = nullptr);

    void sendStep(const StepPacket& p);

signals:
    void connectedOk();
    void configReceived(const ConfigPacket& c);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer server;
    QTcpSocket* socket = nullptr;
    QByteArray buffer;
};

#endif // SERVER_H
