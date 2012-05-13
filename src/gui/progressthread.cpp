#include "progressthread.h"
#include <QTime>

#include <QDebug>

ProgressThread::ProgressThread(QProgressBar *bar, int seconds, QObject *parent) :
    QThread(parent), m_bar(bar), m_seconds(seconds)
{
}

void ProgressThread::run()
{
    QTime t;
    t.start();

    qDebug() << m_seconds;

    while( t.elapsed() < 1000 * m_seconds )
    {
        sleep(300);
        //emit updateValue(t.elapsed() / (10 * m_seconds));
        m_bar->setValue(t.elapsed() / (10 * m_seconds));
    }
    qDebug() << "Exiting Progress Thread";
}
