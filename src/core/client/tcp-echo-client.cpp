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

#include "tcp-echo-client.h"

TcpEchoClient::TcpEchoClient(const QString &localAddr, 
                             const QString &serverAddr, 
                             TcpEchoTask *task)
    : Client(localAddr, serverAddr), m_task(task)
{
}

void TcpEchoClient::run()
{
    LOG_DEBUG("Beginning of TcpEchoClient::run");
    
    // Get basic info & attributes from task
    quint16 serverPort = m_task->getServerPort();
    qint32 startTime = m_task->getStartTime();
    qint32 stopTime = m_task->getStopTime();
    
    const QVector<qint32> &inputSizes = m_task->getInputSizes();
    const QVector<qint32> &echoSizes = m_task->getEchoSizes();
    const QVector<qint32> &intervals = m_task->getIntervals();
    
    // Bind local address and connect
    QBoundTcpSocket socket;
    if( !socket.bindAndConnect(m_localAddr, 0, m_serverAddr, serverPort))
    {
        LOG_ERROR("An error occurred in QBoundTcpSocket");
        return;
    }

    // Sleep before start
    msleep(startTime);
    
    for(int i = 0; i < intervals.size(); i++)
    {
        // Sleep during the interval
        msleep(intervals[i]);
        
        // Send input
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (qint16)qMax(0, inputSizes[i] - 2 - 2);
        out << (qint16)echoSizes[i];
        block.append(QByteArray(qMax(0, inputSizes[i] - 4), 0));
        
        socket.write(block);
        socket.waitForBytesWritten(-1);
        
        // Receive echo
        QTime t;
        t.start();
        
        while( socket.bytesAvailable() < echoSizes[i] )
        {
            if( !socket.waitForReadyRead(3 * 1000))
            {
                if( socket.state() == QAbstractSocket::UnconnectedState )
                {
                    break;
                }
                else
                {
                    LOG_INFO("BulkDownload session timed out");
                }
            }
        }
        
        if( socket.state() == QAbstractSocket::UnconnectedState )
        {
            break;
        }
        else
        {
            socket.read(echoSizes[i]);
            
            m_trace.index.append(i);
            m_trace.time.append(GlobalTimer::msec());
            m_trace.delay.append(t.elapsed());
        }
        
        if( t.elapsed() >= stopTime )
        {
            break;
        }
    }   

    // Close connection
    socket.close();
    LOG_DEBUG("End of TcpEchoClient::run");
}

void TcpEchoClient::generateTextTrace(TextTraceItem &trace)
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

void TcpEchoClient::generateRegularTrace(RegularTraceItem &trace, int seconds)
{
    // Init regular trace
    trace.clear();
    trace.insert("Delay", QList<qint32>());
    trace.insert("Active", QList<qint32>());
    
    // Scan raw trace
    QVector<int> sums(seconds, 0);
    QVector<int> samples(seconds, 0);
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        if( m_trace.time[i] < seconds * 1000 )
        {
            sums[m_trace.time[i] / 1000] += m_trace.delay[i];
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
        trace["Delay"].append((samples[i] == 0) ? 0 : sums[i] / samples[i]);
        trace["Active"].append((samples[i] == 0) ? 0 : 1);
    }
}
