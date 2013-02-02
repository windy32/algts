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

#include "async-udp-echo-client.h"

AsyncUdpEchoClient::AsyncUdpEchoClient(const QString &localAddr, 
                                       const QString &serverAddr, 
                                       AsyncUdpEchoTask *task)
    : Client(localAddr, serverAddr), m_task(task)
{
}

void AsyncUdpEchoClient::run()
{
    LOG_DEBUG("Beginning of AsyncUdpEchoClient::run");
    
    // Get basic info & attributes from task
    quint16 serverPort = m_task->getServerPort();
    qint32 startTime = m_task->getStartTime();
    qint32 stopTime = m_task->getStopTime();
    
    const QVector<qint32> &inputSizes = m_task->getInputSizes();
    const QVector<qint32> &echoSizes = m_task->getEchoSizes();
    const QVector<qint32> &intervals = m_task->getIntervals();
    
    // Bind local address
    QUdpSocket socket;
    if( !socket.bind(QHostAddress(m_localAddr), 0))
    {
        LOG_ERROR("Cannot bind UdpEchoClient");
        return;
    }
    
    QTime t;
    t.start();
    
    // Sleep before start
    msleep(startTime);
    
    // Start receiving thread
    AsyncUdpEchoClientReceiver receiver(&socket, this);
    m_receiving = true;
    receiver.start();
    
    for(int i = 0; i < intervals.size(); i++)
    {
        // Sleep during the interval
        msleep(intervals[i]);

        // Send input
        /// add index field
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (qint32)i;
        out << (qint16)echoSizes[i];
        block.append(QByteArray(qMax(0, inputSizes[i] - 6), 0));
                
        qint64 bytesWritten = socket.writeDatagram(block, 
            QHostAddress(m_serverAddr), serverPort);
        socket.waitForBytesWritten(-1);
        if( bytesWritten == -1 )
        {
            LOG_INFO("Cannot send input packet %d / %d", 
                i + 1, intervals.size());
            return;
        }
        else if( bytesWritten == 0 )
        {
            LOG_DEBUG("No bytes sent for input packet %d / %d", 
                i + 1, intervals.size());
            return;
        }
        
        m_mutex.lock();
        m_trace.index.append(i);
        m_trace.time.append(GlobalTimer::msec());
        m_trace.delay.append(-1);
        m_mutex.unlock();
        
        if( t.elapsed() >= stopTime )
        {
            break;
        }
    }
    
    msleep(3 * 1000);
    m_receiving = false;
    receiver.wait();
    
    LOG_DEBUG("End of AsyncUdpEchoClient::run");
}

void AsyncUdpEchoClient::generateTextTrace(TextTraceItem &trace)
{
    trace.clear();
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        trace.append(QString("%1 %2 %3")
            .arg(m_trace.index[i])
            .arg(m_trace.time[i])
            .arg(m_trace.delay[i]));
    }
}

void AsyncUdpEchoClient::generateRegularTrace(RegularTraceItem &trace, 
                                              int seconds)
{
    // Init regular trace
    trace.clear();
    trace.insert("Delay", QList<qint32>());
    trace.insert("Lost", QList<qint32>());
    trace.insert("Active", QList<qint32>());
    
    // Scan raw trace
    QVector<int> sums(seconds, 0); // sum of echo delays
    QVector<int> losts(seconds, 0); // number of packets lost in the second
    QVector<int> samples(seconds, 0); // total packets sent in the second
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        if( m_trace.time[i] < seconds * 1000 )
        {
            if( m_trace.delay[i] == -1 ) // lost
            {
                losts[m_trace.time[i] / 1000] += 1;
            }
            else
            {
                sums[m_trace.time[i] / 1000] += m_trace.delay[i];
            }
            samples[m_trace.time[i] / 1000] += 1;
        }
        else
        {
            LOG_DEBUG("Time is out of range");
        }
    }
    
    // Save regular result
    for(int i = 0; i < seconds; i++)
    {
        trace["Delay"].append(
            (samples[i] == 0) ? 0 : sums[i] / (samples[i] - losts[i]));
        trace["Lost"].append(
            (samples[i] == 0) ? 0 : losts[i] * 100 / samples[i]); // unit is 1%
        trace["Active"].append(
            (samples[i] == 0) ? 0 : 1);
    }
}

AsyncUdpEchoClientReceiver::AsyncUdpEchoClientReceiver(
    QUdpSocket *socket, AsyncUdpEchoClient *client)
    : m_socket(socket), 
      m_client(client)
{
}

void AsyncUdpEchoClientReceiver::run()
{
    LOG_DEBUG("Beginning of AsyncUdpEchoClientReceiver::run");
    
    char buffer[1500];
    qint64 size;
    
    while( m_client->m_receiving )
    {
        // Receive echo
        if( m_socket->waitForReadyRead(3 * 1000))
        {
            size = m_socket->readDatagram(buffer, 1500, NULL, NULL);
            if( size < 4 )
            {
                LOG_INFO("Size = %d, Corrupt packet", (int)size);
                continue;
            }
            
            qint32 index;
            QByteArray block(buffer, size);
            QDataStream in(&block, QIODevice::ReadOnly);
            in >> index;
            
            // Set delay
            m_client->m_mutex.lock();
            m_client->m_trace.delay[index] = 
                GlobalTimer::msec() - m_client->m_trace.time[index];
            m_client->m_mutex.unlock();
        }
    }
    LOG_DEBUG("End of AsyncUdpEchoClientReceiver::run");
}

void AsyncUdpEchoClient::generateRawTrace(RawTraceItem &trace)
{
    trace.clear();
    trace.insert("Index", QList<qint32>());
    trace.insert("Time", QList<qint32>());
    trace.insert("Delay", QList<qint32>());
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        trace["Index"].append(m_trace.index[i]);
        trace["Time"].append(m_trace.time[i]);
        trace["Delay"].append(m_trace.delay[i]);
    }
}

