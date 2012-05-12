#include "testthread.h"
#include "../core/core.h"

TestThread::TestThread(QList<QHostAddress> &localAddrs,
                       QHostAddress &serverAddr,
                       quint16 serverPort,
                       Scenario *scenario,
                       QObject *parent)
    : QThread(parent),
      m_localAddrs(localAddrs),
      m_serverAddr(serverAddr),
      m_serverPort(serverPort),
      m_scenario(scenario)
{
}

void TestThread::run()
{
    CoreApplication app(m_localAddrs, m_serverAddr, m_serverPort);
    app.exec(m_scenario);


}
