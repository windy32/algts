#include <QtGui/QApplication>
#include <QDebug>

#include "mainwindow.h"

#include "dialog/ipaddrdialog.h"
#include "dialog/emulatordialog.h"
#include "dialog/distributiondialog.h"
#include "dialog/selectscenariodialog.h"
#include "dialog/selectscriptdialog.h"
#include "dialog/savetestdialog.h"
#include "dialog/ratingmethoddialog.h"

#include "globaldatabase.h"

/*
void dbtest()
{
    qDebug() << "Scenarios: " << GlobalDatabase::instance()->getScenarioCount();
    qDebug() << "Scripts: " << GlobalDatabase::instance()->getScriptCount();

    Script s;
    s.text = "Hello\n123\n";
    s.params.append(ScriptParam(1, 4, "180"));
    GlobalDatabase::instance()->addScript(s);

    qDebug() << "Scenarios: " << GlobalDatabase::instance()->getScenarioCount();
    qDebug() << "Scripts: " << GlobalDatabase::instance()->getScriptCount();

    Script ss;
    GlobalDatabase::instance()->getScript(1, ss);
    qDebug() << "Text: " << ss.text;
    qDebug() << "First Param: "
             << ss.params[0].line << ", "
             << ss.params[0].pos << ", "
             << ss.params[0].defValue;
}
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //dbtest();
/*
    QString d;
    DistributionDialog d3(d);
    d3.exec();
    qDebug() << d;
*/
/*
    QString interface;
    QList<QHostAddress> addrs;
    IpAddrDialog d1(interface, addrs);
    d1.show();
*/
/*
    EmulatorDialog d2;
    d2.show();
*/
/*
    SelectScenarioDialog d4;
    d4.show();

    SelectScriptDialog d5;
    d5.show();

    SaveTestDialog d6;
    d6.show();

    RatingMethodDialog d7;
    d7.show();
*/
    GlobalDatabase::destroy();

    return a.exec();
}
