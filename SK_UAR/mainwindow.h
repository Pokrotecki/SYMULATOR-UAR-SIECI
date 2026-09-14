#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Symulator.h"
#include "arxwindow.h"
#include "server.h"
#include "client.h"
#include "MenedzerKonfiguracji.h"
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ustawARXDane(const std::vector<double> &a,
                      const std::vector<double> &b,
                      int opoznienie,
                      double szum,
                      double uMin, double uMax,
                      double yMin, double yMax,
                      bool aktywne);

private slots:
    // Obsługa przycisków i kontrolek UI
    void on_Sin_Button_clicked();
    void on_Square_Button_clicked();
    void on_spinBOX_WzmocK_editingFinished();
    void on_spinBOX_Amplituda_editingFinished();
    void on_spinBOX_Czstotliwosc_editingFinished();
    void on_spinBOX_Td_editingFinished();
    void on_spinBOX_Ti_editingFinished();
    void on_spinBOX_Interwal_editingFinished();
    void on_radio_przed_toggled(bool checked);
    void on_radio_pod_toggled(bool checked);
    void on_Reset_d_clicked();
    void on_Reset_i_clicked();
    void on_START_Button_clicked();
    void on_STOP_Bttun_clicked();
    void on_RESET_Button_clicked();
    void on_Konf_ARX_Button_clicked();
    void on_Zapisz_Button_clicked();
    void on_Wczytaj_Button_clicked();
    void on_spinBox_Wypelnienie_editingFinished();
    void on_SpinBox_Stala_editingFinished();
    void on_spinBoxOknoczasowe_editingFinished();
    //sieciowe
    void wyslijConfigPacket(bool resetHistorii = false);
    void wyslijStepPacket(double w, double y, double e, double u, int k, double P, double I, double D);
    void onTimeoutSieci();
    void onSterowanieReceived(quint32 seq, double u, double w);
    void onOutputReceived(quint32 seq, double y);
    void czyscStareDaneSzybka(double aktualnyCzas);
    void onKrokObiektu(double w, double y, double u, int k);

    //  Główny slot odbierający dane z symulatora
    void onKrokWykonany(double w, double y, double e, double u, int k, double P, double I, double D);

    void on_TrybSieciowy_Button_clicked();

    void on_radio_przed_clicked();



private:
    Ui::MainWindow *ui;
    MenedzerKonfiguracji menedzerKonfig;
    ARXwindow *arxwindow = nullptr;
    SymulatorUAR symulator;

    // zmienne na potrzeby polaczenia sieciowego
    enum tryb
    {
        lokalny,
        obiekt,
        regulator
    };
    tryb Tryb;
    Server* server=nullptr;
    Client* client=nullptr;

    quint32 wyslanySeq = 0;  // seq który został wysłany
    quint32 licznikSpoznien = 0;
    bool pakietNaCzas = true;

    // lampka opoznien
    static const int PROG_OPOZNIEN_POMARANCZOWY = 1;
    static const int PROG_POPRAWY_ZIELONY = 10;
    bool trybDegradacjiSieci = false;

    // do wykrywania faktycznego zerwania polaczenia (przejscie polaczono->rozlaczono)
    bool bylPolaczony = false;
    // stan pracy regulatora (start/stop) przeslany w configu - potrzebny obiektowi, zeby wiedziec czy po powrocie do trybu lokalnego wznowic symulacje, czy nie
    bool regulatorAktywny = false;
    struct ZakresY {
        double minVal =  1e18;
        double maxVal = -1e18;
        bool brudny = false;
    } zakresMain, zakresPid, zakresUchyb, zakresReg;

    //  Wykresy i Serie
    QLineSeries *seriaP;
    QLineSeries *seriaI;
    QLineSeries *seriaD;
    QLineSeries *seriaUchyb;
    QLineSeries *seriaRegulator;
    QLineSeries *seriaRegulowana;
    QLineSeries *seriaZad;

    //  Osie 
    QValueAxis *mainX;
    QValueAxis *mainY;
    QValueAxis *uchybX;
    QValueAxis *uchybY;
    QValueAxis *regX;
    QValueAxis *regY;
    QValueAxis *pidX;
    QValueAxis *pidY;

    // Zmienne pomocnicze do logiki wykresu
    double doceloweOknoCzasowe;
    double aktualnyCzasSymulacji;

    // Bufor punktów czekających na odrysowanie. Rysowanie wykresu jest odseparowane
    // od taktu symulacji/sieci, zeby przy krotkich interwalach GUI
    // mialo czas na obsluge gniazda sieciowego zamiast rysowac wykres co kazdy takt
    struct PunktBufora {
        double t, w, y, e, u, P, I, D;
    };
    std::vector<PunktBufora> buforPunktow;

    struct PunktObiektu {
        double t, w, y, u;
    };
    std::vector<PunktObiektu> buforObiektu;

    QTimer wykresTimer;

    // Dane ARX
    std::vector<double> aktualnyWektorA;
    std::vector<double> aktualnyWektorB;
    int aktualneOpoznienie;
    double aktualnySzum;
    double arx_uMin = -10.0;
    double arx_uMax = 10.0;
    double arx_yMin = -10.0;
    double arx_yMax = 10.0;
    bool arx_ograniczenia = true;

    double aktualnaSzerokoscOkna;
    const double MARGINES_BEZPIECZENSTWA = 0.5;

    //  Funkcje pomocnicze
    void wyczyscWykresy();
    void dopasujSkalePionowa(QValueAxis *osY, QList<QLineSeries*> serie);
    void odswiezWykresy();


    void aktualizujZakresOsiX(double krokAnimacji, double wymaganeOkno, double aktualnyCzas);
    void czyscStareDane(double aktualnyCzas);

    // Funkcje Sieciowe
    void trybLokalny();
    void trybObiektu();
    void trybRegulatora();

    void uruchomKlienta();
    void uruchomSerwer();
    void disconnectNetwork();

    //void statusPolaczeniaOK();
    //void statusPolaczeniaBrak();
    void ukryjStatusPolaczenia();
    void aktualizujStatusSieci();

    void onStepPacketReceivedClient(const StepPacket& p);
    void onConfigPacketReceivedServer(const ConfigPacket& c);
    void onConfigPacketReceivedClient(const ConfigPacket& c);

    void wyslijSterowanie(double u, double w);
};

#endif // MAINWINDOW_H

