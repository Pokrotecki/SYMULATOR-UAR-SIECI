#ifndef CLIENT_H
#define CLIENT_H
#pragma once
#include <QTcpSocket>
#include <QObject>
#include "StepPacket.h"

class Client : public QObject
{
    Q_OBJECT
public:
    Client(const QString& host, quint16 port, QObject* parent = nullptr);

public slots:
    void sendStep(const StepPacket& p);

private slots:
    void onReadyRead();

private:
    QTcpSocket socket;
};
#endif // CLIENT_H
