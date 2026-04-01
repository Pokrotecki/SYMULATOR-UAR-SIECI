#include "mainwindow.h"
#include <QCoreApplication>
#include <QTextStream>

#include "server.h"
#include "client.h"
#include "Symulator.h"
#include <QApplication>
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
*/

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream cin(stdin);

    qDebug() << "Wybierz tryb:";
    qDebug() << "1 - Serwer";
    qDebug() << "2 - Klient";
    qDebug() << "Twój wybór:";

    QString wybor = cin.readLine().trimmed();
    wybor = "1";

    if (wybor == "1")
    {
        qDebug() << "Uruchamiam tryb SERWERA...";
        new Server(5000);
    }
    else if (wybor == "2")
    {
        qDebug() << "Uruchamiam tryb KLIENTA...";
        Client* client = new Client("127.0.0.1", 5000);

        // Twój symulator
        SymulatorUAR* sym = new SymulatorUAR(
            GeneratorSygnalu(),
            RegulatorPID(),
            ModelARX({0}, {0})
            );

        // Połączenie sygnału krokWykonany → wysyłanie pakietu
        QObject::connect(sym, &SymulatorUAR::krokWykonany,[client](double w, double y, double e, double u,int k, double P, double I, double D){StepPacket p{w, y, e, u, k, P, I, D};client->sendStep(p);});

        sym->start();
    }
    else
    {
        qDebug() << "Niepoprawny wybór. Kończę program.";
        return 0;
    }

    return app.exec();
}
