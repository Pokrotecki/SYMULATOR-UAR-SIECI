#ifndef SERVER_H
#define SERVER_H
#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"
#include "ConfigPacket.h"

#include "ControlPacket.h"
#include "OutputPacket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject* parent = nullptr);

    void sendStep(const StepPacket& p);

    void sendOutput(const OutputPacket& p);
    void sendConfig(const ConfigPacket& c);
    QTcpSocket* getSocket() const { return socket; }

signals:
    void connectedOk();
    void disconnected();
    void configReceived(const ConfigPacket& c);
    void sterowanieReceived(quint32 seq, double u, double w);
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
