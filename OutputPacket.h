#pragma once
#include <QDataStream>

struct OutputPacket
{
    quint32 seq;
    double y;
};

inline QDataStream& operator<<(QDataStream& out, const OutputPacket& p)
{
    out << p.seq << p.y;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, OutputPacket& p)
{
    in >> p.seq >> p.y;
    return in;
}
