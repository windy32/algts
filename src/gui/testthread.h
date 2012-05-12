#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QHostAddress>

#include "scenarioex.h"

class TestThread : public QThread
{
    Q_OBJECT

private:
    QList<QHostAddress> &m_localAddrs;
    QHostAddress &m_serverAddr;
    quint16 m_serverPort;
    Scenario *m_scenario;

protected:
    void run();

public:
    explicit TestThread(QList<QHostAddress> &localAddrs,
                        QHostAddress &serverAddr,
                        quint16 serverPort,
                        Scenario *scenario,
                        QObject *parent = 0);
};

#endif // TESTTHREAD_H
