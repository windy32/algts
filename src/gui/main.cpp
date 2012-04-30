#include <QtGui/QApplication>

#include "mainwindow.h"

#include "dialog/ipaddrdialog.h"
#include "dialog/emulatordialog.h"
#include "dialog/distributiondialog.h"
#include "dialog/selectscenariodialog.h"
#include "dialog/selectscriptdialog.h"
#include "dialog/savetestdialog.h"
#include "dialog/ratingmethoddialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

/*
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
*/
    return a.exec();
}
