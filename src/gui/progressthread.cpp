#include "progressthread.h"
#include <QTime>

ProgressThread::ProgressThread(QProgressBar *bar, int seconds, QObject *parent) :
    QThread(parent), m_bar(bar), m_seconds(seconds)
{
}

void ProgressThread::run()
{
    QTime t;
    t.start();

    while( t.elapsed() < 1000 * m_seconds )
    {
        sleep(300);
        m_bar->setValue(t.elapsed() / (10 * m_seconds));
    }
}
