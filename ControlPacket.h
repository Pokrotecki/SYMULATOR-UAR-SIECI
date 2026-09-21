#pragma once
#include <QDataStream>

struct ControlPacket
{
    quint32 seq;  // numer kroku regulatora
    double u;
    double w;
};

inline QDataStream& operator<<(QDataStream& out, const ControlPacket& p)
{
    out << p.seq << p.u << p.w;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ControlPacket& p)
{
    in >> p.seq >> p.u >> p.w;
    return in;
}
