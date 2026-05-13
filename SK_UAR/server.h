#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"
#include "ConfigPacket.h"

//IDK
#include "ControlPacket.h"
#include "OutputPacket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject* parent = nullptr);

    void sendStep(const StepPacket& p);

    //IDK
    void sendOutput(const OutputPacket& p);
    QTcpSocket* getSocket() const { return socket; }

signals:
    void connectedOk();
    void disconnected();
    void configReceived(const ConfigPacket& c);
    //IDK
    void sterowanieReceived(double u, double w);
    //void controlReceived(double u);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer server;
    QTcpSocket* socket = nullptr;
    QByteArray buffer;
};

#endif // SERVER_H
