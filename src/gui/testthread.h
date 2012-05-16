#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QHostAddress>

#include "scenarioex.h"

class TestThread : public QThread
{
    Q_OBJECT

private:
    QList<QHostAddress> m_localAddrs;
    QHostAddress m_serverAddr;
    quint16 m_serverPort;
    ScenarioEx *m_scenario;

    static TestThread *m_instance;

protected:
    void run();

public:
    explicit TestThread(QList<QHostAddress> localAddrs,
                        QHostAddress serverAddr,
                        quint16 serverPort,
                        ScenarioEx *scenario,
                        QObject *parent = 0);

    static void logCallback(const char *content);
signals:
    void newLog(const QString &);
};

#endif // TESTTHREAD_H
