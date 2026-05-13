#pragma once
#include <QDataStream>

struct ControlPacket
{
    double u;
    double w;
};

inline QDataStream& operator<<(QDataStream& out, const ControlPacket& p)
{
    out << p.u << p.w;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, ControlPacket& p)
{
    in >> p.u >> p.w;
    return in;
}
