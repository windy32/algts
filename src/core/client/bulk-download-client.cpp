// Copyright (C) 2012-2013 Fengyu Gao (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
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

#include "bulk-download-client.h"

#include <QDebug>

BulkDownloadClient::BulkDownloadClient(const QString &localAddr, 
                                       const QString &serverAddr, 
                                       BulkDownloadTask *task)
    : Client(localAddr, serverAddr), m_task(task)
{
}

void BulkDownloadClient::run()
{
    LOG_DEBUG("Beginning of BulkDownloadClient::run");
    
    // Get basic info & attributes from task
    quint16 serverPort = m_task->getServerPort();
    qint32 startTime = m_task->getStartTime();
    qint32 stopTime = m_task->getStopTime();
    
    qint32 maxBytes = m_task->getMaxBytes();
    qint32 maxRate = m_task->getMaxRate();

    // Start timer
    QTime t;
    t.start();

    // Wait
    msleep(startTime);

    // First line in raw trace
    m_trace.index.append(0);
    m_trace.time.append(GlobalTimer::msec());
    m_trace.newBytes.append(0);
    m_trace.totalBytes.append(0);

    // Bind local address and connect
    QBoundTcpSocket socket;
    if( !socket.bindAndConnect(m_localAddr, 0, m_serverAddr, serverPort))
    {
        LOG_ERROR("An error occurred in QBoundTcpSocket");
        return;
    }
    
    // Send request
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << maxBytes;
    out << maxRate;
    
    socket.write(block);
    socket.waitForBytesWritten(-1);
    
    // Receive data
    // Until all bytes are recieved, or it exceeds stopTime
    char buffer[256 * 1024];
    qint32 totalBytes = 0;
    qint32 index = 0;
    
    while( t.elapsed() < stopTime )
    {
        if( socket.waitForReadyRead(5 * 1000))
        {
            qint32 bytesRead = (qint32)socket.read(buffer, 256 * 1024);
            totalBytes += bytesRead;
            
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
            LOG_DEBUG("BulkDownload session timed out");
        }
    }
    
    // Server closed connection after sending specific bytes, or it's stop time
    socket.close();
    LOG_DEBUG("End of BulkDownloadClient::run");
}

void BulkDownloadClient::generateTextTrace(TextTraceItem &trace)
{
    trace.clear();
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        trace.append(QString("%1 %2 %3 %4")
            .arg(m_trace.index[i])
            .arg(m_trace.time[i])
            .arg(m_trace.newBytes[i])
            .arg(m_trace.totalBytes[i]));
    }
}

void BulkDownloadClient::generateRegularTrace(RegularTraceItem &trace, 
                                              int seconds)
{
    // Init regular trace
    trace.clear();
    trace.insert("RxRate", QList<qint32>());
    trace.insert("MaxRxRate", QList<qint32>());
    trace.insert("Active", QList<qint32>());
    
    // Calculate busy ratio
    int startTime = m_trace.time.first(); // ms
    int stopTime = m_trace.time.last(); // ms
    QVector<double> busyRatios(seconds, 0.0);

    for(int i = startTime / 1000; i <= stopTime / 1000; i++)
    {
        if( startTime / 1000 == stopTime / 1000 )
        {
            busyRatios[i] += (stopTime - startTime) / 1000.0;
            break;
        }
        if( i >= seconds )
        {
            LOG_DEBUG("Index is out of range");
            break;
        }
        
        if( i == startTime / 1000 )
        {
            busyRatios[i] += 1.0 - (startTime % 1000) / 1000.0;
        }
        else if( i < stopTime / 1000 )
        {
            busyRatios[i] += 1.0;
        }
        else if( i == stopTime / 1000 )
        {
            busyRatios[i] += (stopTime % 1000) / 1000.0;
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

void BulkDownloadClient::generateRawTrace(RawTraceItem &trace)
{
    trace.clear();
    trace.insert("Index", QList<qint32>());
    trace.insert("Time", QList<qint32>());
    trace.insert("NewBytes", QList<qint32>());
    trace.insert("TotalBytes", QList<qint32>());
    
    for(int i = 0; i < m_trace.index.size(); i++)
    {
        trace["Index"].append(m_trace.index[i]);
        trace["Time"].append(m_trace.time[i]);
        trace["NewBytes"].append(m_trace.newBytes[i]);
        trace["TotalBytes"].append(m_trace.totalBytes[i]);
    }
}

