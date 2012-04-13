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
 * \brief The asynchronous udp echo task client
 * \ingroup Clients
 *
 * Regular trace format:
 *  - vector<int32> Delay (ms)
 *  - vector<int32> Lost (%1)
 *  - vector<int32> Active (BOOL)
 *
 * Text trace format:
 *  - index
 *  - time (ms)
 *  - delay (ms)
 *
 * An example of text trace output:
 * \code
 * 0 30 63
 * 1 99 43
 * 2 127 72
 * 3 183 -1 (lost)
 * 4 227 89
 * 5 472 58
 * ...
 * \endcode
 */
class AsyncUdpEchoClient : public Client
{
protected:
    /**
     * \brief Raw trace structure for asynchronous udp echo tasks
     */
    struct RawTrace
    {
        /** \brief Index of the event that starts with 0 */
        QVector<qint32> index;
        
        /** \brief The time (ms) of the event */
        QVector<qint32> time;
        
        /** \brief The delay (ms) of the echo packet
         *  \note a value of -1 indicates that the echo packet is lost
         */
        QVector<qint32> delay;
    };
    
    /**
     * \brief The raw trace object
     */
    RawTrace m_trace;

    /**
     * \brief Pointer to the tcp echo task object
     */
    AsyncUdpEchoTask *m_task;
    
    /**
     * \brief The mutex for visiting raw trace
     */
    QMutex m_mutex;
    
    /**
     * \brief The flag indicating whether the receiver thread should continue
     *        waiting and receiving packets
     */
    bool m_receiving;
    
private:
	virtual void run();
    friend class AsyncUdpEchoClientReceiver;
    
public:
    /**
     * \brief Initialize the asynchronous udp echo client
     * \param localAddr Client's IPv4 address
     * \param serverAddr Server's IPv4 address
     * \param task Pointer to the input task object
     */
    AsyncUdpEchoClient(const QString &localAddr, 
                       const QString &serverAddr, AsyncUdpEchoTask *task);

    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

/**
 * \brief The receiver thread of the asynchronous udp echo client
 *
 * Since the AsyncUdpEchoClient works in asynchronous mode, it's incorrect
 * to send an input packet, wait for the echo, and send next input packet in
 * one thread. It's necessary to create two threads that share a same piece of
 * data, and update the raw trace concurrently.
 *
 * This class is juct the receiver thread. For convenience, this class is set
 * to AsyncUdpEchoClient's friend class.
 *
 * \see AsyncUdpEchoClient
 */
class AsyncUdpEchoClientReceiver : public QThread
{
private:
    QUdpSocket *m_socket;
    AsyncUdpEchoClient *m_client;
    
private:
    virtual void run();

public:
    /**
     * \brief Initialize the receiver thread of the asynchronous udp echo client
     * \param socket The socket of the established connection
     * \param client Pointer to the udp echo client object
     */
    AsyncUdpEchoClientReceiver(QUdpSocket *socket, AsyncUdpEchoClient *client);
};

#endif /* ASYNC_UDP_ECHO_CLIENT_H */

