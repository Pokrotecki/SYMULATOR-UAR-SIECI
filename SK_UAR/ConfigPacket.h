#pragma once
#include <vector>
#include <QDataStream>
#include <QVector>
#include <GeneratorSygnalu.h>
#include <RegulatorPID.h>

struct ConfigPacket
{
    double Kp, Ti, Td;
    RegulatorPID::LiczCalk typCalki;

    double uMin, uMax;

    double A, TRZ, P, S, TT;
    GeneratorSygnalu::Tryb trybGeneratora;

    std::vector<double> arxA;
    std::vector<double> arxB;
    int opoznienie;
    double szum;
    bool ograniczenia;

    double yMin, yMax;

    int interwalMs;
    double oknoCzasowe;

    bool resetHistorii = false; // jednorazowe zdarzenie, opcjonalnie przesylane
    bool symulacjaAktywna = false; // czy regulator ma aktualnie wcisniety start
};

// SERIALIZACJA
inline QDataStream& operator<<(QDataStream& out, const ConfigPacket& c)
{
    out << c.Kp << c.Ti << c.Td << c.typCalki

        << c.uMin << c.uMax

        << c.A << c.TRZ << c.P << c.S << c.TT << c.trybGeneratora

        << QVector<double>(c.arxA.begin(), c.arxA.end())
        << QVector<double>(c.arxB.begin(), c.arxB.end())

        << c.opoznienie << c.szum

        << c.ograniczenia

        << c.yMin << c.yMax

        << c.interwalMs<<c.oknoCzasowe

        << c.resetHistorii << c.symulacjaAktywna;

    return out;
}

// DESERIALIZACJA
inline QDataStream& operator>>(QDataStream& in, ConfigPacket& c)
{
    QVector<double> a, b;

    in >> c.Kp >> c.Ti >> c.Td >> c.typCalki

        >> c.uMin >> c.uMax

        >> c.A >> c.TRZ >> c.P >> c.S >> c.TT >> c.trybGeneratora

        >> a
        >> b

        >> c.opoznienie >> c.szum

        >> c.ograniczenia

        >> c.yMin >> c.yMax

        >> c.interwalMs>>c.oknoCzasowe

        >> c.resetHistorii >> c.symulacjaAktywna;

    c.arxA = std::vector<double>(a.begin(), a.end());
    c.arxB = std::vector<double>(b.begin(), b.end());

    return in;
}

inline ConfigPacket makeConfigPacket(double Kp, double Ti, double Td, RegulatorPID::LiczCalk typCalki,
                                     double uMin,double uMax,
                                     double A, double TRZ, double P, double S, double TT, GeneratorSygnalu::Tryb trybGeneratora,
                                     const std::vector<double>& arxA, const std::vector<double>& arxB,
                                     int opoznienie, double szum, bool ograniczenia,
                                     double yMin, double yMax, int interwalMs, double oknoCzasowe,
                                     bool resetHistorii = false, bool symulacjaAktywna = false)
{
    ConfigPacket c;
    c.Kp = Kp;
    c.Ti = Ti;
    c.Td = Td;
    c.typCalki = typCalki;

    c.uMin = uMin;
    c.uMax = uMax;

    c.A = A;
    c.TRZ = TRZ;
    c.P = P;
    c.S = S;
    c.TT = TT;
    c.trybGeneratora = trybGeneratora;

    c.arxA = arxA;
    c.arxB = arxB;

    c.opoznienie = opoznienie;
    c.szum = szum;
    c.ograniczenia = ograniczenia;

    c.yMin = yMin;
    c.yMax = yMax;

    c.interwalMs = interwalMs;
    c.oknoCzasowe = oknoCzasowe;

    c.resetHistorii = resetHistorii;
    c.symulacjaAktywna = symulacjaAktywna;

    return c;
}
