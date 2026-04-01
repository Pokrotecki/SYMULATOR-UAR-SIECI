#ifndef STEPPACKET_H
#define STEPPACKET_H
#pragma once
#include <QDataStream>

struct StepPacket {
    double w, y, e, u;
    int k;
    double P, I, D;
};

inline QDataStream& operator<<(QDataStream& out, const StepPacket& p) {
    out << p.w << p.y << p.e << p.u << p.k << p.P << p.I << p.D;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, StepPacket& p) {
    in >> p.w >> p.y >> p.e >> p.u >> p.k >> p.P >> p.I >> p.D;
    return in;
}
#endif // STEPPACKET_H
