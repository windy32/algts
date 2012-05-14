#include "progressthread.h"
#include <QTime>

#include <QDebug>

ProgressThread::ProgressThread(int seconds, QObject *parent) :
    QThread(parent), m_seconds(seconds), m_running(true)
{
}

void ProgressThread::run()
{
    QTime t;
    t.start();

    while( m_running && t.elapsed() < 1000 * m_seconds )
    {
        msleep(100);
        emit updateValue(5 * t.elapsed() / (10 * m_seconds));
    }

    if( !m_running ) // Test thread finished earlier than expected
    {
        emit updateValue(500);
    }
}

void ProgressThread::testFinished()
{
    m_running = false;
}
