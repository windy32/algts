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

#include "bulk-download-server.h"

BulkDownloadServer::BulkDownloadServer(const QHostAddress &addr, quint32 port)
    : TcpServer(addr, port)
{
}

TcpServerSession *BulkDownloadServer::createSession(int socketDescriptor)
{
    return new BulkDownloadServerSession(socketDescriptor);
}

BulkDownloadServerSession::BulkDownloadServerSession(int socketDescriptor)
    : TcpServerSession(socketDescriptor)
{
}

void BulkDownloadServerSession::run()
{
    LOG_INFO("Beginning of BulkDownloadServerSession::run");
    
    QTcpSocket socket;
    if( !socket.setSocketDescriptor(m_socketDescriptor))
    {
        LOG_ERROR("Cannot set socket descriptor");
    }

    // Receive request
    qint32 maxBytes;
    qint32 maxRate;
    
    while( socket.bytesAvailable() < 8 )
    {
        if( !socket.waitForReadyRead(5 * 1000))
        {
            LOG_INFO("BulkDownload session timed out");
            return;
        }
    }
    
    QDataStream in(&socket);
    in >> maxBytes >> maxRate;
    
    LOG_DEBUG("MaxBytes = %d, MaxRate = %d", maxBytes, maxRate);
    
    // Send data
    const int packetSize = 1000;
    qint64 bytesSent = 0;
    QByteArray block(packetSize, 0);
    QTime t;
    t.start();
    
    while( maxBytes == -1 || bytesSent < maxBytes )
    {
        socket.write(block);
        socket.waitForBytesWritten(-1);
        bytesSent += 1000;
        
        if( socket.state() == QAbstractSocket::UnconnectedState )
        {
            LOG_DEBUG("Bulk download session closed by client");
            break;
        }
        
        //LOG_DEBUG("BulkDownloadServerSession %d / %d", 
        //    (int)bytesSent, (int)maxBytes);
        
        // Rate limit
        if( maxRate != -1 && bytesSent * 1000 / maxRate  > (qint64)t.elapsed())
        {
            msleep(qMax((int)(bytesSent * 1000 / maxRate  - t.elapsed()), 0));
        }
    }
    
    // Connection closed by client, or all bytes sent
    socket.close();
    LOG_INFO("End of BulkDownloadServerSession::run");
}

