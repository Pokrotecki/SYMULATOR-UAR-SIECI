#ifndef SERVER_H
#define SERVER_H
#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(quint16 port, QObject* parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();

signals:
    void connectedOk();

private:
    QTcpServer server;
    QTcpSocket* socket = nullptr;
};
#endif // SERVER_H
