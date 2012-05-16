#include "testthread.h"
#include "../core/core.h"
#include "gui-application.h"

#include <QDebug>

TestThread *TestThread::m_instance = 0;

TestThread::TestThread(QList<QHostAddress> localAddrs,
                       QHostAddress serverAddr,
                       quint16 serverPort,
                       ScenarioEx *scenario,
                       QObject *parent)
    : QThread(parent),
      m_localAddrs(localAddrs),
      m_serverAddr(serverAddr),
      m_serverPort(serverPort),
      m_scenario(scenario)
{
    m_instance = this;
}

void TestThread::run()
{
    GuiApplication app(m_localAddrs, m_serverAddr, m_serverPort);
    app.exec(m_scenario);
}

void TestThread::logCallback(const char *content)
{
    if( m_instance != NULL ) // This is odd, should be optimized later
    {
        m_instance->emit newLog(QString::fromAscii(content));
    }
}
