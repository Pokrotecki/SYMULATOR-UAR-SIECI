#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"
#include "ConfigPacket.h"

#include "ControlPacket.h"
#include "OutputPacket.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString& host, quint16 port, QObject* parent = nullptr);

    void sendConfig(const ConfigPacket& c);
    //IDK
    void sendControl(quint32 seq, double u, double w);
    QTcpSocket* getSocket() { return &socket; }

signals:
    void connectedOk();
    void disconnected();
    void stepReceived(const StepPacket& p);
    //IDK
    void outputReceived(quint32 seq, double y);
    void configReceived(const ConfigPacket& c);

private slots:
    void onReadyRead();

private:
    QTcpSocket socket;
    QByteArray buffer;
};

#endif // CLIENT_H
