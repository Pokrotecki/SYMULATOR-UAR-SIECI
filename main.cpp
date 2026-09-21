#include "mainwindow.h"
#include <QCoreApplication>
#include <QTextStream>

#include "server.h"
#include "client.h"
#include "Symulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}

/*
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream cin(stdin);

    qDebug() << "Wybierz tryb:";
    qDebug() << "1 - Serwer";
    qDebug() << "2 - Klient";

    QString wybor = cin.readLine().trimmed();
    wybor = "2";

    if (wybor == "1")
    {
        qDebug() << "Uruchamianie trybu SERWERA...";
        new Server(5000);
    }
    else if (wybor == "2")
    {
        qDebug() << "Uruchamianie trybu KLIENTA...";
        Client* client = new Client("10.0.0.30", 5000);

        SymulatorUAR* sym = new SymulatorUAR(
            GeneratorSygnalu(),
            RegulatorPID(),
            ModelARX({0}, {0})
            );

        QObject::connect(sym, &SymulatorUAR::krokWykonany,[client](double w, double y, double e, double u,int k, double P, double I, double D){StepPacket p{w, y, e, u, k, P, I, D};client->sendStep(p);});

        sym->start();
    }
    else
    {
        qDebug() << "Niepoprawny wybór.";
        return 0;
    }

    return app.exec();
}
*/
