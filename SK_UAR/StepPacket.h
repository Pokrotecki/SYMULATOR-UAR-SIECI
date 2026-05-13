#ifndef STEPPACKET_H
#define STEPPACKET_H
#pragma once
#include <QDataStream>
//raczej calosc do usuniecia ale narazie zostawiam
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

inline StepPacket makeStepPacket(double w, double y, double e, double u, int k, double P, double I, double D)
{
    StepPacket p;
    p.w = w;
    p.y = y;
    p.e = e;
    p.u = u;
    p.k = k;
    p.P = P;
    p.I = I;
    p.D = D;
    return p;
}
#endif // STEPPACKET_H
