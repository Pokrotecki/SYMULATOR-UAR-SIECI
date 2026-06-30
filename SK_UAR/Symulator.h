#pragma once

#include <QObject>
#include <QTimer>
#include "GeneratorSygnalu.h"
#include "ModelARX.h"
#include "ProstyUAR.h"
#include "RegulatorPID.h"
#include <vector>
#include <QElapsedTimer>

class SymulatorUAR : public QObject
{
    Q_OBJECT

private:
    // WARSTWA DANYCH
    GeneratorSygnalu generator;
    RegulatorPID pid;
    ModelARX arx;
    ProstyUAR uar;

    // STAN SYMULACJI
    int k;//krok
    double w, e, u, y;//wartość zadana,uchyb regulacji,sygnał sterujący, wyjście obiektu

    //IDK NA POTRZEBY TRYBU SIECIOWEGO
    double ostatnieYsieciowe;
    bool oczekiwanieNaY;
    double ostatniePoprawneU;
    bool trybSieciowyRegulator;
    bool ostatniPakietNaCzas;
    int liczbaSpoznionychPakietow;
    QElapsedTimer timerPakietu;

    // ZEGAR
    bool symuluj;
    int interwalMs;
    QTimer timer;

public:
    // KONSTRUKTOR 
    SymulatorUAR(const GeneratorSygnalu &gen,
                 const RegulatorPID &pid_,
                 const ModelARX &arx_,
                 QObject *parent = nullptr)
        : QObject(parent)
        , generator(gen)
        , pid(pid_)
        , arx(arx_)
        , uar(arx, pid)
        , k(0)
        , w(0.0)
        , e(0.0)
        , u(0.0)
        , y(0.0)
        , ostatnieYsieciowe(0.0) //IDK
        , oczekiwanieNaY(false) //IDK
        , trybSieciowyRegulator(false) //IDK
        , ostatniPakietNaCzas(true) //IDK
        , liczbaSpoznionychPakietow(0) //IDK
        , symuluj(false)
        , interwalMs(200)
    {
        timer.setInterval(interwalMs);//ustawienie czasu timera
        connect(&timer, &QTimer::timeout, this, &SymulatorUAR::Tick);//krok po każdym ticku
    }

    // STEROWANIE SYMULACJĄ
    void start()
    {
        symuluj = true;
        if (!timer.isActive())
            timer.start();
    }

    void stop()
    {
        symuluj = false;
        timer.stop();
    }

    void reset()
    {
        stop();
        k = 0;
        w = e = u = y = 0.0;
        uar.reset();
    }


    // Generator
    void setGeneratorTryb(GeneratorSygnalu::Tryb t) { generator.ustawTryb(t); }
    void setGeneratorA(double a) { generator.ustawA(a); }
    void setGeneratorS(double s) { generator.ustawS(s); }
    void setGeneratorP(double p) { generator.ustawP(p); }
    void setGeneratorTRZ(double trz) { generator.ustawTRZ(trz); }
    void setGeneratorTT(int tt) { generator.ustawTT(tt); }

    void setGeneratorCzestotliwosc(double f)
    {
        if (f > 0)
            setGeneratorTRZ(1.0 / f);
    }

    // Regulator PID
    void setPID_Kp(double kp) { pid.setKp(kp); }
    void setPID_Ti(double ti) { pid.setStalaCalk(ti); }
    void setPID_Td(double td) { pid.setTd(td); }
    void setPID_T(double t) { pid.setT(t); }
    void setPID_TypCalki(RegulatorPID::LiczCalk typ) { pid.setLiczCalk(typ); }
    void PID_resetCalki(){pid.reset_calki();}

    // ARX
    void setARX(const std::vector<double> &a,
                const std::vector<double> &b,
                int opoznienie,
                double szum)
    {
        arx.ustawParametry(a, b, opoznienie, szum);

    }
    void setARX_Umin(double umin) { arx.setUmin(umin); }
    void setARX_Umax(double umax) { arx.setUmax(umax); }
    void setARX_Ymin(double ymin) { arx.setYmin(ymin); }
    void setARX_Ymax(double ymax) { arx.setYmax(ymax); }
    void setPID_Umin(double umin) { pid.setUmin(umin); }
    void setPID_Umax(double umax) { pid.setUmax(umax); }
    void setARX_Ograniczenia(bool aktywne) {arx.setOgraniczenia(aktywne);}
    void setPID_Ograniczenia(bool aktywne) {pid.setOgraniczenia(aktywne);}


    // Ręczny krok symulacji
    void krokSymulacji()
    {
        if (symuluj) {
            uar.krok(w, e, u, y, generator, k);
            k++;
        }
    }

    // GETTERY
    int getKrok() const { return k; }
    double getWartoscZadana() const { return w; }
    double getUchyb() const { return e; }
    double getSterowanie() const { return u; }
    double getWyjscie() const { return y; }
    //na potrzeby komunikacji sieciowej
    bool czyPakietyNaCzas() const { return ostatniPakietNaCzas;}
    int getLiczbaSpoznien() const {return liczbaSpoznionychPakietow; }
    GeneratorSygnalu::Tryb getGeneratorTryb() const { return generator.getTryb(); }

    int getInterwalMs() const { return interwalMs; }
    void setInterwalMs(int ms)
    {
        if (ms < 1)
            ms = 1;
        interwalMs = ms;
        timer.setInterval(ms);
    }

    bool czysymuluj() const { return symuluj; }
    //dostęp do składniowych PID
    double getP() const { return pid.P; }
    double getI() const { return pid.I; }
    double getD() const { return pid.D; }

    //IDK
    double symulujObiekt(double uSterujace)
    {
        y = arx.symuluj(uSterujace);
        return y;
    }
    void ustawYsieciowe(double nowey)
    {
        ostatnieYsieciowe = nowey;
        oczekiwanieNaY = false;
    }

    bool czyOczekiwanieNaY() const
    {
        return oczekiwanieNaY;
    }
    void setTrybSieciowyRegulator(bool v)
    {
        trybSieciowyRegulator = v;
    }
    void krokSieciowyObiektu(double uSterujace, double wartoscZadana){
        u = uSterujace;
        w = wartoscZadana;

        y = arx.symuluj(u);

        e = w - y;

        emit krokWykonany(
            w,
            y,
            e,
            u,
            k,
            0.0,
            0.0,
            0.0
            );

        k++;
    }

signals:
    //emitowanie sygnałów dla GUI
    void krokWykonany(double w, double y, double e, double u, int k, double P, double I, double D);

    //IDK
    void wyslijSterowanie(double u, double w);
    // jezeli za duzo opoznien
    void timeoutSieci();

private slots:
    void Tick()
    {
        if (!symuluj)
            return;

        // TRYB LOKALNY
        if (!trybSieciowyRegulator)
        {
            uar.krok(w, e, u, y, generator, k);
            double wartP = getP();
            double wartI = getI();
            double wartD = getD();
            emit krokWykonany(w, y, e, u, k, wartP, wartI, wartD);
            k++;
            return;
        }

        // TRYB REGULATORA SIECIOWEGO

        // Sprawdź czy odpowiedź na poprzedni krok wróciła
        if (oczekiwanieNaY)
        {
            // Y nie wróciło przed kolejnym tickiem - spóźnienie
            ostatniPakietNaCzas = false;
            liczbaSpoznionychPakietow++;

            if (liczbaSpoznionychPakietow >= 10)
            {
                emit timeoutSieci();
                return;
            }

            // kontynuuj na ostatnim znanym Y
            y = ostatnieYsieciowe;
        }
        else
        {
            // Y wróciło na czas
            ostatniPakietNaCzas = true;
            liczbaSpoznionychPakietow = 0;
            y = ostatnieYsieciowe;  // użyj świeżego Y które właśnie przyszło
        }

        // Zawsze generuj w i licz PID
        w = generator.generuj(k);
        e = w - y;
        u = pid.symuluj(e);

        // Wyślij sterowanie i zaznacz że czekamy na odpowiedź
        oczekiwanieNaY = true;
        emit wyslijSterowanie(u, w);

        double wartP = getP();
        double wartI = getI();
        double wartD = getD();

        emit krokWykonany(w, y, e, u, k, wartP, wartI, wartD);
        k++;
    }
};

