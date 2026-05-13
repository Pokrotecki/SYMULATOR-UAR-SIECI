#pragma once
#include <QDataStream>

struct OutputPacket
{
    double y;
};

inline QDataStream& operator<<(QDataStream& out, const OutputPacket& p)
{
    out << p.y;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, OutputPacket& p)
{
    in >> p.y;
    return in;
}
