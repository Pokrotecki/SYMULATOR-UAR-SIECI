#include "mainwindow.h"
#include "Symulator.h"
#include "arxwindow.h"
#include "qvalueaxis.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDebug>

#include <QInputDialog>
#include <QLineEdit>

//KONSTRUKTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , symulator(GeneratorSygnalu(), RegulatorPID(), ModelARX({0}, {0}))
    , doceloweOknoCzasowe(10.0)
    , aktualnyCzasSymulacji(0.0)
    , aktualnyWektorA({-0.4})
    , aktualnyWektorB({0.6})
    , aktualneOpoznienie(1)
    , aktualnySzum(0.0)
    , arx_uMin(-10.0)
    , arx_uMax(10.0)
    , arx_yMin(-10.0)
    , arx_yMax(10.0)
    , arx_ograniczenia(true)
{
    ui->setupUi(this);
    this->showMaximized();

    Tryb = lokalny;

    on_spinBOX_WzmocK_editingFinished();
    on_spinBOX_Amplituda_editingFinished();
    on_spinBOX_Czstotliwosc_editingFinished();
    on_spinBOX_Td_editingFinished();
    on_spinBOX_Ti_editingFinished();
    on_spinBOX_Interwal_editingFinished();
    on_spinBox_Wypelnienie_editingFinished();
    on_SpinBox_Stala_editingFinished();
    //on_TrybSieciowy_Button_clicked();

    doceloweOknoCzasowe = ui->spinBoxOknoczasowe->value();
    aktualnaSzerokoscOkna = doceloweOknoCzasowe;

    //Serie
    seriaP = new QLineSeries();
    seriaI = new QLineSeries();
    seriaD = new QLineSeries();
    seriaUchyb = new QLineSeries();
    seriaRegulator = new QLineSeries();
    seriaZad = new QLineSeries();
    seriaRegulowana = new QLineSeries();

    //Tytuły
    seriaZad->setName("Wartość zadana");
    seriaRegulowana->setName("Wartość regulowana");
    seriaP->setName("P");
    seriaI->setName("I");
    seriaD->setName("D");
    seriaUchyb->setName("Uchyb");
    seriaRegulator->setName("Sterowanie u");

    //Połączenie z symulatorem
    connect(&symulator, &SymulatorUAR::krokWykonany, this, &MainWindow::onKrokWykonany);

    //Konfiguracja Wykresu Głównego
    QChart *Mainchart = new QChart();
    QChartView *MainchartView = new QChartView(Mainchart);
    Mainchart->setAnimationOptions(QChart::NoAnimation);
    MainchartView->setMinimumSize(600, 400);
    Mainchart->addSeries(seriaZad);
    Mainchart->addSeries(seriaRegulowana);
    Mainchart->setTitle("Zadana i Regulowana");

    mainX = new QValueAxis();
    mainY = new QValueAxis();
    mainX->setTitleText("Czas [s]");
    mainY->setTitleText("Wartość");
    mainX->setTickCount(11);
    Mainchart->addAxis(mainX, Qt::AlignBottom);
    Mainchart->addAxis(mainY, Qt::AlignLeft);
    seriaZad->attachAxis(mainX); seriaZad->attachAxis(mainY);
    seriaRegulowana->attachAxis(mainX); seriaRegulowana->attachAxis(mainY);

    MainchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_5->addWidget(MainchartView);

    //Konfiguracja Wykresu PID
    QChart *PIDchart = new QChart();
    QChartView *PIDchartView = new QChartView(PIDchart);
    PIDchart->setAnimationOptions(QChart::NoAnimation);
    PIDchart->addSeries(seriaP);
    PIDchart->addSeries(seriaI);
    PIDchart->addSeries(seriaD);
    PIDchart->setTitle("Składowe sterowania PID");

    pidX = new QValueAxis();
    pidY = new QValueAxis();
    pidX->setTitleText("Czas [s]");
    pidY->setTitleText("Wartość");
    PIDchart->addAxis(pidX, Qt::AlignBottom);
    PIDchart->addAxis(pidY, Qt::AlignLeft);
    seriaP->attachAxis(pidX); seriaP->attachAxis(pidY);
    seriaI->attachAxis(pidX); seriaI->attachAxis(pidY);
    seriaD->attachAxis(pidX); seriaD->attachAxis(pidY);

    PIDchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_4->addWidget(PIDchartView, 1);

    //Konfiguracja Wykresu Uchybu
    QChart *Uchybchart = new QChart();
    QChartView *UchybchartView = new QChartView(Uchybchart);
    Uchybchart->setAnimationOptions(QChart::NoAnimation);
    Uchybchart->addSeries(seriaUchyb);
    Uchybchart->setTitle("Uchyb");

    uchybX = new QValueAxis();
    uchybY = new QValueAxis();
    uchybX->setTitleText("Czas [s]");
    uchybY->setTitleText("Wartość");
    Uchybchart->addAxis(uchybX, Qt::AlignBottom);
    Uchybchart->addAxis(uchybY, Qt::AlignLeft);
    seriaUchyb->attachAxis(uchybX); seriaUchyb->attachAxis(uchybY);

    UchybchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_4->addWidget(UchybchartView, 1);

    //Konfiguracja Wykresu Regulatora
    QChart *Regulatorchart = new QChart();
    QChartView *RegulatorchartView = new QChartView(Regulatorchart);
    Regulatorchart->setAnimationOptions(QChart::NoAnimation);
    Regulatorchart->addSeries(seriaRegulator);
    Regulatorchart->setTitle("Regulator");

    regX = new QValueAxis();
    regY = new QValueAxis();
    regX->setTitleText("Czas [s]");
    regY->setTitleText("Wartość");
    Regulatorchart->addAxis(regX, Qt::AlignBottom);
    Regulatorchart->addAxis(regY, Qt::AlignLeft);
    seriaRegulator->attachAxis(regX); seriaRegulator->attachAxis(regY);

    RegulatorchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_4->addWidget(RegulatorchartView, 1);
    RegulatorchartView->setMinimumSize(0, 300);


    //dodane aby domyslnie startowalo z danymi
    ustawARXDane(
        aktualnyWektorA,
        aktualnyWektorB,
        aktualneOpoznienie,
        aktualnySzum,
        arx_uMin,
        arx_uMax,
        arx_yMin,
        arx_yMax,
        arx_ograniczenia
        );
    // schowanie funkcjonalnosci sieciowej
    ui->StatusPolaczenia_Label->hide();
}

void MainWindow::aktualizujZakresOsiX(double krokAnimacji, double wymaganeOkno, double aktualnyCzas)
{
    // Dazenie do zadanego okna
    if (aktualnaSzerokoscOkna < wymaganeOkno) {
        aktualnaSzerokoscOkna = std::min(wymaganeOkno, aktualnaSzerokoscOkna + krokAnimacji);
    }
    else if (aktualnaSzerokoscOkna > wymaganeOkno) {
        aktualnaSzerokoscOkna = std::max(wymaganeOkno, aktualnaSzerokoscOkna - krokAnimacji);
    }

    // Obliczenie startu osi. Jeśli czas < okno startujemy od 0.
    double poczatekOsi = (aktualnyCzas > aktualnaSzerokoscOkna) ? (aktualnyCzas - aktualnaSzerokoscOkna) : 0.0;

    // Ustawienie zakresow dla wszystkich osi
    if(mainX) mainX->setRange(poczatekOsi, aktualnyCzas);
    if(pidX) pidX->setRange(poczatekOsi, aktualnyCzas);
    if(uchybX) uchybX->setRange(poczatekOsi, aktualnyCzas);
    if(regX) regX->setRange(poczatekOsi, aktualnyCzas);
}

void MainWindow::czyscStareDane(double aktualnyCzas)
{

    if (!seriaZad || seriaZad->count() == 0) return;
    double maxHistoria = ui->spinBoxOknoczasowe->maximum();

    // Granice usuwania
    double progOdciecia = aktualnyCzas - maxHistoria - MARGINES_BEZPIECZENSTWA;


    while (seriaZad->count() > 0 && seriaZad->at(0).x() < progOdciecia)
    {
        seriaZad->remove(0);
        seriaRegulowana->remove(0);
        if(seriaP->count() > 0) seriaP->remove(0);
        if(seriaI->count() > 0) seriaI->remove(0);
        if(seriaD->count() > 0) seriaD->remove(0);
        if(seriaUchyb->count() > 0) seriaUchyb->remove(0);
        if(seriaRegulator->count() > 0) seriaRegulator->remove(0);
    }
}
/* stara wersja, wolniejsza (?)
void MainWindow::onKrokWykonany(double w, double y, double e, double u, int k, double P, double I, double D)
{
    double dt = symulator.getInterwalMs() / 1000.0;

    if (k == 0) aktualnyCzasSymulacji = 0.0;
    else aktualnyCzasSymulacji += dt;

    double t = aktualnyCzasSymulacji;

    //Dodawanie punktow
    seriaZad->append(t, w);
    seriaRegulowana->append(t, y);
    seriaP->append(t, P);
    seriaI->append(t, I);
    seriaD->append(t, D);
    seriaUchyb->append(t, e);
    seriaRegulator->append(t, u);

    // Obsługa plynnej osi X
    // 0.2 to predkosc animacji zmiany okna
    aktualizujZakresOsiX(0.2, doceloweOknoCzasowe, t);

    // Usuwanie starych danych
    czyscStareDane(t);


    if (k % 2 == 0) {
        dopasujSkalePionowa(mainY, {seriaZad, seriaRegulowana});
        dopasujSkalePionowa(pidY, {seriaP, seriaI, seriaD});
        dopasujSkalePionowa(uchybY, {seriaUchyb});
        dopasujSkalePionowa(regY, {seriaRegulator});
    }

    if(Tryb!=tryb::lokalny){
        aktualizujStatusSieci();
    }
}
*/

void MainWindow::czyscStareDaneSzybka(double aktualnyCzas)
{
    if (!seriaZad || seriaZad->count() == 0) return;

    double maxHistoria = ui->spinBoxOknoczasowe->maximum();
    double progOdciecia = aktualnyCzas - maxHistoria - MARGINES_BEZPIECZENSTWA;

    // Policz ile punktów wypadło poza bufor
    int n = 0;
    int ile = seriaZad->count();
    while (n < ile && seriaZad->at(n).x() < progOdciecia)
        ++n;

    if (n == 0) return;

    // Jedno wywołanie zamiast n-krotnego remove(0)
    seriaZad->removePoints(0, n);
    seriaRegulowana->removePoints(0, std::min(n, seriaRegulowana->count()));
    if (seriaP->count()        >= n) seriaP->removePoints(0, n);
    if (seriaI->count()        >= n) seriaI->removePoints(0, n);
    if (seriaD->count()        >= n) seriaD->removePoints(0, n);
    if (seriaUchyb->count()    >= n) seriaUchyb->removePoints(0, n);
    if (seriaRegulator->count()>= n) seriaRegulator->removePoints(0, n);

    // Rescan zakresów Y — tylko po faktycznym usunięciu punktów
    auto rescan1 = [](QLineSeries* s, double& mn, double& mx) {
        mn =  1e18; mx = -1e18;
        for (int i = 0; i < s->count(); ++i) {
            double v = s->at(i).y();
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
    };
    auto merge = [](QLineSeries* s, double& mn, double& mx) {
        for (int i = 0; i < s->count(); ++i) {
            double v = s->at(i).y();
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
    };

    rescan1(seriaZad,      zakresMain.minVal,   zakresMain.maxVal);
    merge(seriaRegulowana, zakresMain.minVal,   zakresMain.maxVal);
    zakresMain.brudny = true;

    rescan1(seriaP,        zakresPid.minVal,    zakresPid.maxVal);
    merge(seriaI,          zakresPid.minVal,    zakresPid.maxVal);
    merge(seriaD,          zakresPid.minVal,    zakresPid.maxVal);
    zakresPid.brudny = true;

    rescan1(seriaUchyb,    zakresUchyb.minVal,  zakresUchyb.maxVal);
    zakresUchyb.brudny = true;

    rescan1(seriaRegulator,zakresReg.minVal,    zakresReg.maxVal);
    zakresReg.brudny = true;
}

void MainWindow::onKrokObiektu(double w, double y, int k) // FIX wydzielic pozniej wspolna funkcjonalnosc do osobnej funckji - duplikacja duzej ilosci kodu, ++++ cos nie dziala skalowanie osi Y
{
    double dt = symulator.getInterwalMs() / 1000.0;
    if (k == 0) aktualnyCzasSymulacji = 0.0;
    else        aktualnyCzasSymulacji += dt;
    double t = aktualnyCzasSymulacji;

    // Tylko dwie serie
    seriaZad->append(t, w);
    seriaRegulowana->append(t, y);

    // Aktualizuj zakres Y przyrostowo
    if (w < zakresMain.minVal) { zakresMain.minVal = w; zakresMain.brudny = true; }
    if (w > zakresMain.maxVal) { zakresMain.maxVal = w; zakresMain.brudny = true; }
    if (y < zakresMain.minVal) { zakresMain.minVal = y; zakresMain.brudny = true; }
    if (y > zakresMain.maxVal) { zakresMain.maxVal = y; zakresMain.brudny = true; }

    // Oś X
    if (aktualnaSzerokoscOkna < doceloweOknoCzasowe)
        aktualnaSzerokoscOkna = std::min(doceloweOknoCzasowe, aktualnaSzerokoscOkna + 0.2);
    else if (aktualnaSzerokoscOkna > doceloweOknoCzasowe)
        aktualnaSzerokoscOkna = std::max(doceloweOknoCzasowe, aktualnaSzerokoscOkna - 0.2);

    double poczatek = (t > aktualnaSzerokoscOkna) ? (t - aktualnaSzerokoscOkna) : 0.0;
    if (mainX) mainX->setRange(poczatek, t);

    // Czyszczenie starych danych — tylko seriaZad i seriaRegulowana
    if (seriaZad->count() > 0) {
        double maxHistoria = ui->spinBoxOknoczasowe->maximum();
        double prog = t - maxHistoria - MARGINES_BEZPIECZENSTWA;
        int n = 0;
        while (n < seriaZad->count() && seriaZad->at(n).x() < prog) ++n;
        if (n > 0) {
            seriaZad->removePoints(0, n);
            seriaRegulowana->removePoints(0, std::min(n, seriaRegulowana->count()));
            // Po usunięciu — rescan tylko tych dwóch serii, co 20 kroków
            zakresMain.brudny = true;
        }
    }

    if (k % 20 == 0 && zakresMain.brudny) {
        zakresMain.minVal = 1e18; zakresMain.maxVal = -1e18;
        for (int i = 0; i < seriaZad->count(); ++i) {
            double v = seriaZad->at(i).y();
            if (v < zakresMain.minVal) zakresMain.minVal = v;
            if (v > zakresMain.maxVal) zakresMain.maxVal = v;
        }
        for (int i = 0; i < seriaRegulowana->count(); ++i) {
            double v = seriaRegulowana->at(i).y();
            if (v < zakresMain.minVal) zakresMain.minVal = v;
            if (v > zakresMain.maxVal) zakresMain.maxVal = v;
        }
    }

    if (mainY && zakresMain.brudny && k % 2 == 0) {
        double margines = (zakresMain.maxVal - zakresMain.minVal) * 0.1;
        if (margines < 0.01) margines = 1.0;
        mainY->setRange(zakresMain.minVal - margines, zakresMain.maxVal + margines);
        zakresMain.brudny = false;
    }

    //if (Tryb != tryb::lokalny && k % 10 == 0)
    //    aktualizujStatusSieci();
}

void MainWindow::onKrokWykonany(double w, double y, double e,
                                double u, int k,
                                double P, double I, double D)
{
    double dt = symulator.getInterwalMs() / 1000.0;
    if (k == 0) aktualnyCzasSymulacji = 0.0;
    else        aktualnyCzasSymulacji += dt;
    double t = aktualnyCzasSymulacji;

    // 1. Dodaj punkty (zawsze, każdy krok)
    seriaZad->append(t, w);
    seriaRegulowana->append(t, y);
    seriaP->append(t, P);
    seriaI->append(t, I);
    seriaD->append(t, D);
    seriaUchyb->append(t, e);
    seriaRegulator->append(t, u);

    // 2. Aktualizuj zakresy Y przyrostowo (bez skanowania serii)
    auto aktualizujY = [](ZakresY& z, std::initializer_list<double> vals) {
        for (double v : vals) {
            if (v < z.minVal) { z.minVal = v; z.brudny = true; }
            if (v > z.maxVal) { z.maxVal = v; z.brudny = true; }
        }
    };
    aktualizujY(zakresMain,  {w, y});
    aktualizujY(zakresPid,   {P, I, D});
    aktualizujY(zakresUchyb, {e});
    aktualizujY(zakresReg,   {u});

    // 3. Usuń stare punkty (jedno removePoints zamiast pętli remove(0))
    czyscStareDaneSzybka(t);

    // 4. Oś X — co krok (tania operacja, samo setRange)
    if (aktualnaSzerokoscOkna < doceloweOknoCzasowe)
        aktualnaSzerokoscOkna = std::min(doceloweOknoCzasowe,
                                         aktualnaSzerokoscOkna + 0.2);
    else if (aktualnaSzerokoscOkna > doceloweOknoCzasowe)
        aktualnaSzerokoscOkna = std::max(doceloweOknoCzasowe,
                                         aktualnaSzerokoscOkna - 0.2);

    double poczatek = (t > aktualnaSzerokoscOkna)
                          ? (t - aktualnaSzerokoscOkna) : 0.0;

    if (mainX)  mainX->setRange(poczatek, t);
    if (pidX)   pidX->setRange(poczatek, t);
    if (uchybX) uchybX->setRange(poczatek, t);
    if (regX)   regX->setRange(poczatek, t);

    // 5. Oś Y — co 2 kroki i tylko gdy dane się zmieniły
    if (k % 2 == 0) {
        auto zastosujY = [](QValueAxis* os, ZakresY& z) {
            if (!os || !z.brudny) return;
            double margines = (z.maxVal - z.minVal) * 0.1;
            if (margines < 0.01) margines = 1.0;
            os->setRange(z.minVal - margines, z.maxVal + margines);
            z.brudny = false;
        };
        zastosujY(mainY,  zakresMain);
        zastosujY(pidY,   zakresPid);
        zastosujY(uchybY, zakresUchyb);
        zastosujY(regY,   zakresReg);
    }

    // 6. Status sieci
    //if (Tryb != tryb::lokalny)
    //   aktualizujStatusSieci();
}

void MainWindow::wyczyscWykresy()
{
    seriaZad->clear();
    seriaRegulowana->clear();
    seriaP->clear();
    seriaI->clear();
    seriaD->clear();
    seriaUchyb->clear();
    seriaRegulator->clear();

    aktualnyCzasSymulacji = 0.0;

    zakresMain  = { 1e18, -1e18, false };
    zakresPid   = { 1e18, -1e18, false };
    zakresUchyb = { 1e18, -1e18, false };
    zakresReg   = { 1e18, -1e18, false };

    mainX->setRange(0, doceloweOknoCzasowe);
    pidX->setRange(0, doceloweOknoCzasowe);
    uchybX->setRange(0, doceloweOknoCzasowe);
    regX->setRange(0, doceloweOknoCzasowe);
}

void MainWindow::dopasujSkalePionowa(QValueAxis *osY, QList<QLineSeries*> serie)
{
    double minVal = 999999.0;
    double maxVal = -999999.0;
    bool znaleziono = false;


    double xMinWidoczne = mainX->min();
    double xMaxWidoczne = mainX->max();

    for (QLineSeries *s : serie) {
        if(!s) continue;
        QList<QPointF> punkty = s->points();


        for (int i = punkty.size() - 1; i >= 0; --i) {
            double x = punkty[i].x();
            double y = punkty[i].y();


            if (x < xMinWidoczne) break;

            if (x <= xMaxWidoczne) {
                if (y < minVal) minVal = y;
                if (y > maxVal) maxVal = y;
                znaleziono = true;
            }
        }
    }

    if (znaleziono) {

        double margines = (maxVal - minVal) * 0.1; // Marginesy 10%
        if (margines < 0.01) margines = 1.0;
        osY->setRange(minVal - margines, maxVal + margines);
    }
}

// ZMIANA OKNA CZASOWEGO
void MainWindow::on_spinBoxOknoczasowe_editingFinished()
{
    doceloweOknoCzasowe = ui->spinBoxOknoczasowe->value();


    double t = aktualnyCzasSymulacji;

    if (t > 0) {
        double minX = 0;
        double maxX = std::max(t, doceloweOknoCzasowe);

        if (t > doceloweOknoCzasowe) {
            minX = t - doceloweOknoCzasowe;
            maxX = t;
        }

        mainX->setRange(minX, maxX);
        pidX->setRange(minX, maxX);
        uchybX->setRange(minX, maxX);
        regX->setRange(minX, maxX);
    }
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

// CZYSZCZENIE WYKRESÓW
/* stara wersja
void MainWindow::wyczyscWykresy()
{
    seriaZad->clear();
    seriaRegulowana->clear();
    seriaP->clear();
    seriaI->clear();
    seriaD->clear();
    seriaUchyb->clear();
    seriaRegulator->clear();

    aktualnyCzasSymulacji = 0.0;

    // Reset osi do domyślnego stanu
    mainX->setRange(0, doceloweOknoCzasowe);
    pidX->setRange(0, doceloweOknoCzasowe);
    uchybX->setRange(0, doceloweOknoCzasowe);
    regX->setRange(0, doceloweOknoCzasowe);
}
*/

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Sin_Button_clicked()
{
    symulator.setGeneratorTryb(GeneratorSygnalu::SINUS);
    if(Tryb == regulator && client)
    {
    wyslijConfigPacket();
    }
}

void MainWindow::on_Square_Button_clicked()
{
    symulator.setGeneratorTryb(GeneratorSygnalu::PROSTOKAT);
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_spinBOX_WzmocK_editingFinished()
{
    symulator.setPID_Kp(ui->spinBOX_WzmocK->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }

}

void MainWindow::on_spinBOX_Amplituda_editingFinished()
{
    symulator.setGeneratorA(ui->spinBOX_Amplituda->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }

}

void MainWindow::on_spinBOX_Czstotliwosc_editingFinished()
{
    symulator.setGeneratorTRZ(ui->spinBOX_Czstotliwosc->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_spinBox_Wypelnienie_editingFinished()
{
    symulator.setGeneratorP(ui->spinBox_Wypelnienie->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_SpinBox_Stala_editingFinished()
{
    symulator.setGeneratorS(ui->SpinBox_Stala->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_spinBOX_Td_editingFinished()
{
    symulator.setPID_Td(ui->spinBOX_Td->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_spinBOX_Ti_editingFinished()
{
    symulator.setPID_Ti(ui->spinBOX_Ti->value());
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }

}

void MainWindow::on_spinBOX_Interwal_editingFinished()
{
    int nowyInterwal = ui->spinBOX_Interwal->value();


    symulator.setGeneratorTT(nowyInterwal);
    symulator.setInterwalMs(nowyInterwal);
    symulator.setPID_T(nowyInterwal / 1000.0);
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }

}

void MainWindow::on_radio_przed_toggled(bool checked)
{
    if (checked)
        symulator.setPID_TypCalki(RegulatorPID::Zew);

    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_radio_pod_toggled(bool checked)
{
    if (checked)
        symulator.setPID_TypCalki(RegulatorPID::Wew);

    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_Reset_d_clicked()
{
    symulator.setPID_Td(0);
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_Reset_i_clicked()
{
    //symulator.setPID_Ti(0);
    symulator.PID_resetCalki();
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_START_Button_clicked()
{
    symulator.start();
}

void MainWindow::on_STOP_Bttun_clicked()
{
    symulator.stop();
}

void MainWindow::on_RESET_Button_clicked()
{
    symulator.reset();


    symulator.setPID_Kp(0.5);
    ui->spinBOX_WzmocK->setValue(0.5);
    symulator.setPID_Ti(5.0);
    ui->spinBOX_Ti->setValue(5.0);
    symulator.setPID_Td(0.2);
    ui->spinBOX_Td->setValue(0.2);

    symulator.setGeneratorA(1.0);
    ui->spinBOX_Amplituda->setValue(1.0);
    symulator.setGeneratorTRZ(3.0);
    ui->spinBOX_Czstotliwosc->setValue(3.0);
    symulator.setGeneratorTT(50);
    ui->spinBOX_Interwal->setValue(50);
    symulator.setGeneratorS(0.0);
    ui->SpinBox_Stala->setValue(0.0);
    symulator.setGeneratorP(0.5);
    ui->spinBox_Wypelnienie->setValue(0.5);

    symulator.setInterwalMs(50);
    symulator.setPID_T(0.2);

    symulator.setPID_TypCalki(RegulatorPID::Zew);
    ui->radio_przed->setChecked(true);
    ui->radio_pod->setChecked(false);

    aktualnyWektorA = {-0.4};
    aktualnyWektorB = {0.6};
    aktualneOpoznienie = 1;
    aktualnySzum = 0.0;
    symulator.setARX(aktualnyWektorA, aktualnyWektorB, aktualneOpoznienie, aktualnySzum);
    arx_uMax = 10.0;
    arx_uMin = -10.0;
    arx_yMax = 10.0;
    arx_yMin = -10.0;
    symulator.setPID_Umin(-10.0);
    symulator.setPID_Umax(10.0);
    symulator.setARX_Ymax(10.0);
    symulator.setARX_Ymin(-10.0);
    symulator.setPID_Ograniczenia(true);

    wyczyscWykresy();
    if(Tryb == regulator && client)
    {
        wyslijConfigPacket();
    }
}

void MainWindow::ustawARXDane(const std::vector<double> &a,
                              const std::vector<double> &b,
                              int opoznienie,
                              double szum,
                              double uMin, double uMax,
                              double yMin, double yMax,
                              bool aktywne)
{
    aktualnyWektorA = a;
    aktualnyWektorB = b;
    aktualneOpoznienie = opoznienie;
    aktualnySzum = szum;
    arx_uMin = uMin;
    arx_uMax = uMax;
    arx_yMin = yMin;
    arx_yMax = yMax;
    arx_ograniczenia = aktywne;

    symulator.setARX(a, b, opoznienie, szum);
    symulator.setARX_Umin(uMin);
    symulator.setARX_Umax(uMax);
    symulator.setARX_Ymin(yMin);
    symulator.setARX_Ymax(yMax);
    symulator.setARX_Ograniczenia(aktywne);

    symulator.setPID_Umin(uMin);
    symulator.setPID_Umax(uMax);
    symulator.setPID_Ograniczenia(aktywne);


    if(Tryb == regulator && client)
        wyslijConfigPacket();
}

void MainWindow::on_Konf_ARX_Button_clicked()
{
    if (!arxwindow) {
        arxwindow = new ARXwindow(this);
        connect(arxwindow, &ARXwindow::zatwierdzonoARX, this, &MainWindow::ustawARXDane);
    }
    arxwindow->ustawDane(aktualnyWektorA, aktualnyWektorB, aktualneOpoznienie, aktualnySzum,
                         arx_uMin, arx_uMax, arx_yMin, arx_yMax, arx_ograniczenia);
    arxwindow->show();
    arxwindow->raise();
    arxwindow->activateWindow();

    if(Tryb == regulator && client) //IDK czy dobre miejsce
    {
        wyslijConfigPacket();
    }
}

void MainWindow::on_Zapisz_Button_clicked()
{
    QString sciezka = QFileDialog::getSaveFileName(this, "Zapisz konfigurację", "", "JSON (*.json)");
    if (sciezka.isEmpty()) return;

    bool sukces = menedzerKonfig.zapiszKonfiguracje(
        sciezka,
        aktualnyWektorA,
        aktualnyWektorB,
        aktualneOpoznienie,
        aktualnySzum,
        arx_uMin, arx_uMax,
        arx_yMin, arx_yMax,
        ui->spinBOX_WzmocK->value(),
        ui->spinBOX_Ti->value(),
        ui->spinBOX_Td->value(),
        ui->radio_przed->isChecked() ? 0 : 1,
        ui->Sin_Button->isChecked() ? 0 : 1,
        ui->spinBOX_Amplituda->value(),
        ui->spinBOX_Czstotliwosc->value(),
        ui->SpinBox_Stala->value(),
        ui->spinBox_Wypelnienie->value(),
        ui->spinBOX_Interwal->value()
        );

    if (sukces)
        QMessageBox::information(this, "Sukces", "Konfiguracja zapisana");
    else
        QMessageBox::warning(this, "Błąd", "Nie udało się zapisać");
}

/*
void MainWindow::on_Wczytaj_Button_clicked() //stara wersja do usuniecia po przejsciu checklisty calej
{
    QString sciezka = QFileDialog::getOpenFileName(this, "Wczytaj konfigurację", "", "JSON (*.json)");
        if (sciezka.isEmpty()) return;

        std::vector<double> a, b;
        int opoznienie;
        double odchylenie, Kp, Ti, Td;
        int typCalki, trybGeneratora;
        double amplituda, StalaSkladniowa, Wypelnienie, czestotliwosc;
        int interwalMs;
        double uMin, uMax, yMin, yMax;

        bool sukces = menedzerKonfig.wczytajKonfiguracje(
            sciezka, a, b, opoznienie, odchylenie,
            uMin, uMax, yMin, yMax,
            Kp, Ti, Td, typCalki,
            trybGeneratora, amplituda, czestotliwosc,
            StalaSkladniowa, Wypelnienie, interwalMs
            );

        if (sukces)
        {
            aktualnyWektorA = a;
            aktualnyWektorB = b;
            aktualneOpoznienie = opoznienie;
            aktualnySzum = odchylenie;
            arx_uMin = uMin;
            arx_uMax = uMax;
            arx_yMin = yMin;
            arx_yMax = yMax;

            symulator.setARX(a, b, opoznienie, odchylenie);
            symulator.setARX_Umin(uMin);
            symulator.setARX_Umax(uMax);
            symulator.setARX_Ymin(yMin);
            symulator.setARX_Ymax(yMax);
            symulator.setPID_Umin(uMin);
            symulator.setPID_Umax(uMax);

        on_spinBOX_Interwal_editingFinished();
        on_spinBOX_WzmocK_editingFinished();

        QMessageBox::information(this, "sukces", "konfiguracja wczytana");
        }
        else
        {
            QMessageBox::warning(this, "blad", "nie udało sie wczytac");
        }

}

*/

void MainWindow::on_Wczytaj_Button_clicked()
{
    QString sciezka = QFileDialog::getOpenFileName(this,
                                                   "Wczytaj konfigurację",
                                                   "",
                                                   "JSON (*.json)");
    if (sciezka.isEmpty())
        return;

    std::vector<double> a, b;
    int opoznienie;
    double odchylenie, Kp, Ti, Td;
    int typCalki, trybGeneratora;
    double amplituda, StalaSkladniowa, Wypelnienie, czestotliwosc;
    int interwalMs;
    double uMin, uMax, yMin, yMax;

    bool sukces = menedzerKonfig.wczytajKonfiguracje(
        sciezka,a,b,opoznienie,odchylenie,
        uMin,uMax,yMin,yMax,
        Kp,Ti,Td,typCalki,trybGeneratora,
        amplituda,czestotliwosc,StalaSkladniowa,Wypelnienie,
        interwalMs);

    if (sukces) {
        aktualnyWektorA = a;
        aktualnyWektorB = b;
        aktualneOpoznienie = opoznienie;
        aktualnySzum = odchylenie;
        arx_uMin = uMin;
        arx_uMax = uMax;
        arx_yMin = yMin;
        arx_yMax = yMax;

        symulator.setARX(a, b, opoznienie, odchylenie);
        symulator.setARX_Umin(uMin);
        symulator.setARX_Umax(uMax);
        symulator.setARX_Ymin(yMin);
        symulator.setARX_Ymax(yMax);
        symulator.setPID_Umin(uMin);
        symulator.setPID_Umax(uMax);
        //dodane
        symulator.setPID_Td(Td);
        symulator.setPID_Ti(Ti);
        symulator.setPID_Kp(Kp);

        ui->spinBOX_WzmocK->setValue(Kp);
        ui->spinBOX_Ti->setValue(Ti);
        ui->spinBOX_Td->setValue(Td);


        on_spinBOX_Interwal_editingFinished();
        on_spinBOX_WzmocK_editingFinished();

        QMessageBox::information(this, "sukces", "konfiguracja wczytana");
        wyslijConfigPacket();
    } else {
        QMessageBox::warning(this, "blad", "nie udało sie wczytac");
    }
}


void MainWindow::on_TrybSieciowy_Button_clicked()
{
    if(Tryb == lokalny)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Wybór trybu");
        msgBox.setText("Wybierz tryb pracy aplikacji:");

        QPushButton* btnObiekt = msgBox.addButton("Tryb obiektu", QMessageBox::AcceptRole);
        QPushButton* btnRegulator = msgBox.addButton("Tryb regulatora", QMessageBox::AcceptRole);
        QPushButton* btnAnuluj = msgBox.addButton("Anuluj", QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == btnObiekt)
        {
            uruchomSerwer();
        }
        else if (msgBox.clickedButton() == btnRegulator)
        {
            uruchomKlienta();
        }
    }
    else
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(
            this,
            "Powrót",
            "Powrócić do trybu lokalnego?",
            QMessageBox::Yes | QMessageBox::No
            );

        if(reply == QMessageBox::Yes)
        {

            if(server)
            {
                server->deleteLater();
                server = nullptr;
            }

            if(client)
            {
                client->deleteLater();
                client = nullptr;
            }


            trybLokalny();
        }
    }
}

void MainWindow::uruchomSerwer()
{
    bool ok;

    int port = QInputDialog::getInt(
        this,
        "Port",
        "Podaj port serwera:",
        5000,      // domyślny
        1,
        65535,
        1,
        &ok
        );

    if(!ok)
        return;

    server = new Server(port, this);

    //statusPolaczeniaBrak(); do usuniecia raczej
    aktualizujStatusSieci();

    //connect(server, &Server::connectedOk,this, &MainWindow::statusPolaczeniaOK);
    // KONTROLKA STATUS SIECI
    connect(server, &Server::connectedOk,this, &MainWindow::aktualizujStatusSieci);
    connect(server, &Server::disconnected,this, &MainWindow::aktualizujStatusSieci);

    connect(server, &Server::configReceived, this, &MainWindow::onConfigPacketReceivedServer);
    // update wykresow dla obiektu
    connect(&symulator, &SymulatorUAR::krokObiektu,this, &MainWindow::onKrokObiektu);
    //connect(&symulator, &SymulatorUAR::krokWykonany, this, &MainWindow::wyslijStepPacket); IDK to nie bo wysyla cale dane a obiekt ma lokalnie se liczyc
    // IDK dodane aby wyliczac pomiedzy oboma info
    connect(server,&Server::sterowanieReceived,this,&MainWindow::onSterowanieReceived);
    //IDK zmiana sposobu liczenia w symulatorze
    symulator.setTrybSieciowyRegulator(false);

    QMessageBox::information(
        this,
        "Serwer",
        "Uruchomiono serwer.\nOczekiwanie na klienta."
        );

    trybObiektu();
}

//IDK wyliczanie glownego sterowania
void MainWindow::onSterowanieReceived(quint32 seq , double u, double w)
{
    //qDebug() << "MAINWINDOW: liczę obiekt dla u =" << u;

    //double y = symulator.symulujObiekt(u);
    symulator.krokSieciowyObiektu(u, w);

    OutputPacket p;
    //p.y = y;
    p.seq = seq; //echo kroku (to smao)
    p.y = symulator.getWyjscie();

    server->sendOutput(p);
}

void MainWindow::uruchomKlienta()
{
    bool ok;

    QString ip = QInputDialog::getText(
        this,
        "Połączenie",
        "Podaj adres IP serwera:",
        QLineEdit::Normal,
        "127.0.0.1",
        &ok
        );

    if(!ok || ip.isEmpty())
        return;

    int port = QInputDialog::getInt(
        this,
        "Port",
        "Podaj port serwera:",
        5000,
        1,
        65535,
        1,
        &ok
        );

    if(!ok)
        return;

    client = new Client(ip, port, this);

    //statusPolaczeniaBrak();
    aktualizujStatusSieci();

    //connect(client, &Client::connectedOk,this, &MainWindow::statusPolaczeniaOK);
    //
    connect(client, &Client::connectedOk,this, &MainWindow::aktualizujStatusSieci);
    connect(client, &Client::disconnected,this, &MainWindow::aktualizujStatusSieci);


    //connect(client, &Client::stepReceived,this, &MainWindow::onStepPacketReceivedClient); IDK to raczej nie potrzebne
    //IDK
    connect(&symulator, &SymulatorUAR::wyslijSterowanie, this, &MainWindow::wyslijSterowanie);
    //IDK zmiana sposobu wyliczania symulacji w symulatorze
    symulator.setTrybSieciowyRegulator(true);
    //IDK
    connect(client, &Client::outputReceived,this, &MainWindow::onOutputReceived);

    connect(&symulator, &SymulatorUAR::timeoutSieci,this, &MainWindow::onTimeoutSieci);

    trybRegulatora();
}

void MainWindow::trybLokalny()
{
    Tryb = lokalny;

    ui->TrybSieciowy_Button->setText("TRYB SIECIOWY");
    ui->RESET_Button->setEnabled(true);
    ui->Konf_ARX_Button->setEnabled(true);
    ui->START_Button->setEnabled(true);
    ui->RESET_Button->setEnabled(true);
    ui->Square_Button->setEnabled(true);
    ui->Sin_Button->setEnabled(true);
    ui->Zapisz_Button->setEnabled(true);
    ui->Wczytaj_Button->setEnabled(true);
    ui->STOP_Bttun->setEnabled(true);
    ui->Reset_d->setEnabled(true);
    ui->Reset_i->setEnabled(true);
    ui->SpinBox_Stala->setEnabled(true);
    ui->spinBOX_Amplituda->setEnabled(true);
    ui->spinBOX_Czstotliwosc->setEnabled(true);
    ui->spinBOX_Interwal->setEnabled(true);
    ui->spinBOX_Td->setEnabled(true);
    ui->spinBOX_Ti->setEnabled(true);
    ui->spinBOX_WzmocK->setEnabled(true);
    ui->spinBoxOknoczasowe->setEnabled(true);
    ui->spinBox_Wypelnienie->setEnabled(true);
    ui->radio_pod->setEnabled(true);
    ui->radio_przed->setEnabled(true);

    ukryjStatusPolaczenia();
}

void MainWindow::trybRegulatora()
{
    Tryb = regulator;
    ui->Konf_ARX_Button->setEnabled(false);
    ui->TrybSieciowy_Button->setText("Powrot do trybu lokalnego");
}

void MainWindow::trybObiektu()
{
    Tryb = obiekt;
    ui->TrybSieciowy_Button->setText("Powrot do trybu lokalnego");
    ui->RESET_Button->setEnabled(false);
    //ui->Konf_ARX_Button->setEnabled(false); IDK
    ui->START_Button->setEnabled(false);
    ui->RESET_Button->setEnabled(false);
    ui->Square_Button->setEnabled(false);
    ui->Sin_Button->setEnabled(false);
    ui->Zapisz_Button->setEnabled(false);
    ui->Wczytaj_Button->setEnabled(false);
    ui->STOP_Bttun->setEnabled(false);
    ui->Reset_d->setEnabled(false);
    ui->Reset_i->setEnabled(false);
    ui->SpinBox_Stala->setEnabled(false);
    ui->spinBOX_Amplituda->setEnabled(false);
    ui->spinBOX_Czstotliwosc->setEnabled(false);
    ui->spinBOX_Interwal->setEnabled(false);
    ui->spinBOX_Td->setEnabled(false);
    ui->spinBOX_Ti->setEnabled(false);
    ui->spinBOX_WzmocK->setEnabled(false);
    ui->spinBoxOknoczasowe->setEnabled(false);
    ui->spinBox_Wypelnienie->setEnabled(false);
    ui->radio_pod->setEnabled(false);
    ui->radio_przed->setEnabled(false);
}


// na potrzeby kontrolki statusu polaczenia
/*
void MainWindow::statusPolaczeniaOK()
{
    ui->StatusPolaczenia_Label->show();
    ui->StatusPolaczenia_Label->setText("Status połączenia: nawiązano");
    ui->StatusPolaczenia_Label->setStyleSheet("background-color: green;");
}

void MainWindow::statusPolaczeniaBrak()
{
    ui->StatusPolaczenia_Label->show();
    ui->StatusPolaczenia_Label->setText("Status połączenia: brak");
    ui->StatusPolaczenia_Label->setStyleSheet("background-color: red;");
}
*/


void MainWindow::ukryjStatusPolaczenia()
{
    ui->StatusPolaczenia_Label->hide();
}

void MainWindow::aktualizujStatusSieci()
{
    // brak trybu sieciowego
    if(!client && !server)
    {
        ui->StatusPolaczenia_Label->hide();
        return;
    }

    ui->StatusPolaczenia_Label->show();

    bool polaczono = false;

    // SERVER
    if(server && server->getSocket())
    {
        polaczono = (server->getSocket()->state() == QAbstractSocket::ConnectedState);
    }

    // CLIENT
    if(client)
    {
        polaczono = (client->getSocket()->state() == QAbstractSocket::ConnectedState);
    }

    // BRAK POŁĄCZENIA
    if(!polaczono)
    {
        ui->StatusPolaczenia_Label->setText("Brak połączenia");
        ui->StatusPolaczenia_Label->setStyleSheet(
            "background-color: red;"
            "color: white;"
            "border-radius: 8px;"
            "padding: 4px;"
            );

        return;
    }

    // OPÓŹNIENIA
    if(!symulator.czyPakietyNaCzas())
    {
        ui->StatusPolaczenia_Label->setText("Połączono - opóźnienia");
        ui->StatusPolaczenia_Label->setStyleSheet(
            "background-color: orange;"
            "color: black;"
            "border-radius: 8px;"
            "padding: 4px;"
            );

        return;
    }

    // OK
    ui->StatusPolaczenia_Label->setText("Połączono - synchronizacja OK");
    ui->StatusPolaczenia_Label->setStyleSheet(
        "background-color: green;"
        "color: white;"
        "border-radius: 8px;"
        "padding: 4px;"
        );
}

void MainWindow::onStepPacketReceivedClient(const StepPacket& p)
{
    onKrokWykonany(p.w, p.y, p.e, p.u, p.k, p.P, p.I, p.D);
} //  prawdopodobnie bedzie nie potrzebne pozniej
//IDK
void MainWindow::onOutputReceived(quint32 seq ,double y)
{
    if (seq != wyslanySeq) {
        // spóźniony lub zduplikowany pakiet - ignoruj
        //qDebug() << "Odrzucono pakiet seq=" << seq<< "oczekiwano=" << wyslanySeq;
        return;
    }
    // pakiet na czas
    //qDebug() << "zaakceptowano pakiet seq=" << seq<< "oczekiwano=" << wyslanySeq;
    licznikSpoznien = 0;
    symulator.ustawYsieciowe(y);
    symulator.setCzyPakietNaCzas(true);
    //pakietNaCzas = true;
    //aktualizujStatusSieci();
}

void MainWindow::onConfigPacketReceivedServer(const ConfigPacket& c)
{
    symulator.setPID_Kp(c.Kp);
    symulator.setPID_Ti(c.Ti);
    symulator.setPID_Td(c.Td);

    ui->spinBOX_WzmocK->setValue(c.Kp);
    ui->spinBOX_Ti->setValue(c.Ti);
    ui->spinBOX_Td->setValue(c.Td);

    symulator.setGeneratorA(c.A);
    symulator.setGeneratorTRZ(c.TRZ);
    symulator.setGeneratorP(c.P);
    symulator.setGeneratorS(c.S);
    //symulator.setGeneratorTT(c.TT); powielone pozniej?

    ui->spinBOX_Amplituda->setValue(c.A);
    ui->spinBOX_Czstotliwosc->setValue(c.TRZ);
    ui->spinBox_Wypelnienie->setValue(c.P);
    ui->SpinBox_Stala->setValue(c.S);

    symulator.setARX(c.arxA, c.arxB, c.opoznienie, c.szum);
    symulator.setARX_Ograniczenia(c.ograniczenia);
    symulator.setPID_Ograniczenia(c.ograniczenia);

    // te same 3 operacje co w editingFinished narazie brak lepszego pomyslu poza powieleniem ich
    symulator.setGeneratorTT(c.interwalMs);
    symulator.setInterwalMs(c.interwalMs);
    symulator.setPID_T(c.interwalMs / 1000.0);
    ui->spinBOX_Interwal->setValue(c.interwalMs);  // refresh UI

    // te same operacje co w on_spinBoxOknoczasowe_editingFinished
    doceloweOknoCzasowe = c.oknoCzasowe;
    ui->spinBoxOknoczasowe->setValue(c.oknoCzasowe);  // refreshh UI
    //aktualnaSzerokoscOkna = c.oknoCzasowe;

    double t = aktualnyCzasSymulacji;
    if (t > 0) {
        double minX = 0;
        double maxX = std::max(t, doceloweOknoCzasowe);
        if (t > doceloweOknoCzasowe) {
            minX = t - doceloweOknoCzasowe;
            maxX = t;
        }
        mainX->setRange(minX, maxX);
        pidX->setRange(minX, maxX);
        uchybX->setRange(minX, maxX);
        regX->setRange(minX, maxX);
    }
}

void MainWindow::wyslijSterowanie(double u, double w)
{
    wyslanySeq++;
    symulator.setCzyPakietNaCzas(false);
    //pakietNaCzas = false;   // czekanie na odpowiedz z tym seq
    client->sendControl(wyslanySeq, u, w);
}

void MainWindow::wyslijConfigPacket()
{
    RegulatorPID::LiczCalk trybCalki;
    if(ui->radio_pod->isChecked())
    {
        trybCalki = RegulatorPID::Zew;
    }
    else
    {
        trybCalki = RegulatorPID::Wew;
    }
    ConfigPacket c = makeConfigPacket(
        ui->spinBOX_WzmocK->value(),
        ui->spinBOX_Ti->value(),
        ui->spinBOX_Td->value(),
        trybCalki, //dodany tryb calki
        arx_uMin, arx_uMax, //zamienione placeholdery min i max
        ui->spinBOX_Amplituda->value(),
        ui->spinBOX_Czstotliwosc->value(),
        ui->spinBox_Wypelnienie->value(),
        ui->SpinBox_Stala->value(),
        ui->spinBOX_Interwal->value(),
        symulator.getGeneratorTryb(), //zamieniony placeholder trybu generatora
        aktualnyWektorA,
        aktualnyWektorB,
        aktualneOpoznienie,
        aktualnySzum,
        arx_ograniczenia,
        arx_yMin,
        arx_yMax,
        ui->spinBOX_Interwal->value(),
        ui->spinBoxOknoczasowe->value()
        );
    if(client){
    client->sendConfig(c);
    }else if(server){
        //przesyl arx aby lokalnie byly dane? FIX
    }
}

void MainWindow::wyslijStepPacket(double w, double y, double e, double u, int k, double P, double I, double D)
{
    StepPacket p = makeStepPacket(w, y, e, u, k, P, I, D);
    server->sendStep(p);
}

void MainWindow::onTimeoutSieci()
{
    symulator.setTrybSieciowyRegulator(false);

    if (client) { client->deleteLater(); client = nullptr; }
    if (server) { server->deleteLater(); server = nullptr; }

    trybLokalny();

    QMessageBox::critical(this, "Błąd sieci",
                          "Brak odpowiedzi przez 4 takty z rzędu.\n"
                          "Symulacja kontynuowana w trybie lokalnym.");
}


void MainWindow::on_radio_przed_clicked()
{

}



