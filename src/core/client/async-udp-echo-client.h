// Copyright (C) 2012 Gao Fengyu (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef ASYNC_UDP_ECHO_CLIENT_H
#define ASYNC_UDP_ECHO_CLIENT_H

#include "client.h"
#include "../task/async-udp-echo-task.h"

/**
 * Trace Format of Asynchronous UDP Echo Task
 *
 * Regular trace items for rating gui
 *
 *  - vector [Regular Delay: ms, int32]
 *  - vector [Regular Lost: %1, int32]
 *  - vector [Regular Active, BOOL, int32]
 *
 * Irregular trace items for script outputs
 *
 * vector of vector {index, time(ms), delay(ms)}, e.g.: 
 * 
 *    0, 30, 63
 *    1, 99, 43
 *    2, 127, 72
 *    3, 183, -1 (lost)
 *    4, 227, 89
 *    5, 472, 58
 *    ...
 */

class AsyncUdpEchoClient : public Client
{
private:
    struct RawTrace
    {
        QVector<qint32> index;
        QVector<qint32> time;
        QVector<qint32> delay;
    } m_trace;

	AsyncUdpEchoTask *m_task;
    QMutex m_mutex;
    bool m_receiving;
    
private:
	virtual void run();
    friend class AsyncUdpEchoClientReceiver;
    
public:
    AsyncUdpEchoClient(const QString &localAddr, const QString &serverAddr);

    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

class AsyncUdpEchoClientReceiver : public QThread
{
private:
    QUdpSocket *m_socket;
    AsyncUdpEchoClient *m_client;
    
private:
    virtual void run();

public:
    AsyncUdpEchoClientReceiver(QUdpSocket *socket, AsyncUdpEchoClient *client);
};

#endif /* ASYNC_UDP_ECHO_CLIENT_H */
