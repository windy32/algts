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

#include "onoff-download-client.h"

OnoffDownloadClient::OnoffDownloadClient(const QString &localAddr, 
                                         const QString &serverAddr, 
                                         OnoffDownloadTask *task)
    : Client(localAddr, serverAddr), m_task(task)
{
}

void OnoffDownloadClient::run()
{
    LOG_DEBUG("Beginning of OnoffDownloadClient::run");
    
    // Get basic info & attributes from task
    quint16 serverPort = m_task->getServerPort();
    qint32 startTime = m_task->getStartTime();
    qint32 stopTime = m_task->getStopTime();
    
    qint32 maxRate = m_task->getMaxRate();
    qint16 packetSize = m_task->getPacketSize();
    qint16 requestSize = m_task->getRequestSize();

    const QVector<qint32> &onTimes = m_task->getOnTimes();
    const QVector<qint32> &offTimes = m_task->getOffTimes();

    // Start timer
    QTime t;
    t.start();

    // Wait
    msleep(startTime);

    for(int i = 0; i < onTimes.size(); i++)
    {
        LOG_DEBUG("Period %d / %d", i + 1, onTimes.size());
        
        // Sleep during the off time
        msleep(offTimes[i]);

        // First line in raw trace for current period
        m_trace.period.append(i);
        m_trace.index.append(0);
        m_trace.time.append(GlobalTimer::msec());
        m_trace.newBytes.append(0);
        m_trace.totalBytes.append(0);

        // Bind local address and connect
        LOG_DEBUG("Connect to on/off download server...", offTimes[i]);
        
        QBoundTcpSocket socket;
        if( !socket.bindAndConnect(m_localAddr, 0, m_serverAddr, serverPort))
        {
            LOG_ERROR("An error occurred in QBoundTcpSocket");
            return;
        }
        
        // Send request
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (qint16)qMax(0, requestSize - 2 - 4 - 4 - 2);
        out << onTimes[i];
        out << maxRate;
        out << packetSize; 
        block.append(QByteArray(qMax(0, requestSize - 12), 0));
        
        socket.write(block);
        socket.waitForBytesWritten(-1);
        
        // Receive until server closed connection
        char buffer[256 * 1024];
        qint32 totalBytes = 0;
        qint32 index = 0;

        while( t.elapsed() < stopTime )
        {
            if( socket.waitForReadyRead(5 * 1000))
            {
                qint32 bytesRead = (qint32)socket.read(buffer, 256 * 1024);
                totalBytes += bytesRead;
                
                m_trace.period.append(i);
                m_trace.index.append(++index);
                m_trace.time.append(GlobalTimer::msec());
                m_trace.newBytes.append(bytesRead);
                m_trace.totalBytes.append(totalBytes);
            }
            else if( socket.state() == QAbstractSocket::UnconnectedState )
            {
                break;
            }
            else
            {
                LOG_INFO("OnoffDownload session timed out");
            }
        }
        
        // Close connection
        socket.close();
    }
    LOG_DEBUG("End of OnoffDownloadClient::run");
}

void OnoffDownloadClient::generateTextTrace(TextTraceItem &trace)
{
    trace.clear();
    
    for(int i = 0; i < m_trace.period.size(); i++)
    {
        trace.append(QString("%1 %2 %3 %4 %5")
            .arg(m_trace.period[i])
            .arg(m_trace.index[i])
            .arg(m_trace.time[i])
            .arg(m_trace.newBytes[i])
            .arg(m_trace.totalBytes[i]));
    }
}

void OnoffDownloadClient::generateRegularTrace(RegularTraceItem &trace, int seconds)
{
    // Init regular trace
    trace.clear();
    trace.insert("RxRate", QList<qint32>());
    trace.insert("MaxRxRate", QList<qint32>());
    trace.insert("Active", QList<qint32>());
    
    // Get the start time and the stop time of each on period
    QList<int> startTimes;
    QList<int> stopTimes;
    for(int i = 0; i < m_trace.period.size(); i++)
    {
        if( m_trace.index[i] == 0 ) // start time
        {
            startTimes.append(m_trace.time[i]);
            stopTimes.append(m_trace.time[i]);
        }
        else
        {
            stopTimes.last() = m_trace.time[i];
        }
    }
    
    // Calculate idle ratio
    QVector<double> busyRatios(seconds, 0.0);
    
    for(int i = 0; i < startTimes.size(); i++)
    {
        for(int t = startTimes[i] / 1000; t <= stopTimes[i] / 1000; t++)
        {
            if( startTimes[i] / 1000 == stopTimes[i] / 1000 )
            {
                busyRatios[t] += (stopTimes[i] - startTimes[i]) / 1000.0;
                break;
            }
            if( t >= seconds )
            {
                LOG_DEBUG("Index is out of range");
                break;
            }
            
            if( t == startTimes[i] / 1000 )
            {
                busyRatios[t] += 1.0 - (startTimes[i] % 1000) / 1000.0;
            }
            else if( t < stopTimes[i] / 1000 )
            {
                busyRatios[t] += 1.0;
            }
            else if( t == stopTimes[i] / 1000 )
            {
                busyRatios[t] += (stopTimes[i] % 1000) / 1000.0;
            }
        }
    }

    // MaxRxRate & Active
    for(int i = 0; i < seconds; i++)
    {
        trace["MaxRxRate"].append(
            (m_task->getMaxRate() == -1) ? (busyRatios[i] > 0.0 ? -1 : 0) : 
            busyRatios[i] * m_task->getMaxRate());
        trace["Active"].append(busyRatios[i] > 0.0 ? 1 : 0);
    }
    
    // RxRate
    QVector<qint32> bytesPerSecond(seconds, 0);
    for(int i = 0; i < m_trace.totalBytes.size(); i++)
    {
        if( m_trace.time[i] < seconds * 1000 )
        {
            bytesPerSecond[m_trace.time[i] / 1000] += m_trace.newBytes[i];
        }
        else
        {
            LOG_DEBUG("Time is out of range");
        }
    }
    for(int i = 0; i < seconds; i++)
    {
        trace["RxRate"].append(bytesPerSecond[i]);
    }
}
