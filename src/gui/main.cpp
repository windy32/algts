#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ipaddrdialog.h"
#include "emulatordialog.h"
#include "distributiondialog.h"
#include "selectscenariodialog.h"
#include "selectscriptdialog.h"
#include "savetestdialog.h"
#include "ratingmethoddialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    IpAddrDialog d1;
    d1.show();

    EmulatorDialog d2;
    d2.show();

    DistributionDialog d3;
    d3.show();

    SelectScenarioDialog d4;
    d4.show();

    SelectScriptDialog d5;
    d5.show();

    SaveTestDialog d6;
    d6.show();

    RatingMethodDialog d7;
    d7.show();
    
    return a.exec();
}
