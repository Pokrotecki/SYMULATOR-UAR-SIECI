#pragma once
#include <vector>
#include <QDataStream>
#include <QVector>

struct ConfigPacket
{
    double Kp, Ti, Td;
    double A, TRZ, P, S, TT;

    std::vector<double> arxA;
    std::vector<double> arxB;
    int opoznienie;
    double szum;
    bool ograniczenia;
};

// SERIALIZACJA → zapis do QDataStream
inline QDataStream& operator<<(QDataStream& out, const ConfigPacket& c)
{
    out << c.Kp << c.Ti << c.Td
        << c.A << c.TRZ << c.P << c.S << c.TT
        << QVector<double>(c.arxA.begin(), c.arxA.end())
        << QVector<double>(c.arxB.begin(), c.arxB.end())
        << c.opoznienie
        << c.szum
        << c.ograniczenia;

    return out;
}

// DESERIALIZACJA → odczyt z QDataStream
inline QDataStream& operator>>(QDataStream& in, ConfigPacket& c)
{
    QVector<double> a, b;

    in >> c.Kp >> c.Ti >> c.Td
        >> c.A >> c.TRZ >> c.P >> c.S >> c.TT
        >> a >> b
        >> c.opoznienie
        >> c.szum
        >> c.ograniczenia;

    c.arxA = std::vector<double>(a.begin(), a.end());
    c.arxB = std::vector<double>(b.begin(), b.end());

    return in;
}

inline ConfigPacket makeConfigPacket(double Kp, double Ti, double Td, double A, double TRZ, double P, double S, double TT, const std::vector<double>& arxA, const std::vector<double>& arxB, int opoznienie, double szum, bool ograniczenia)
{
    ConfigPacket c;
    c.Kp = Kp;
    c.Ti = Ti;
    c.Td = Td;

    c.A = A;
    c.TRZ = TRZ;
    c.P = P;
    c.S = S;
    c.TT = TT;

    c.arxA = arxA;
    c.arxB = arxB;

    c.opoznienie = opoznienie;
    c.szum = szum;
    c.ograniczenia = ograniczenia;

    return c;
}
