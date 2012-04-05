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

#include "bulk-download-server.h"

BulkDownloadServer::BulkDownloadServer(const QHostAddress &addr, quint32 port)
    : TcpServer(addr, port)
{
}

TcpServerSession *BulkDownloadServer::createSession(QTcpSocket *socket)
{
    return new BulkDownloadServerSession(socket);
}

BulkDownloadServerSession::BulkDownloadServerSession(QTcpSocket *socket)
    : TcpServerSession(socket)
{
}

void BulkDownloadServerSession::run()
{
    LOG_DEBUG("Beginning of BulkDownloadServerSession::run");

    // Receive request
    qint32 maxBytes;
    qint32 maxRate;
    
    while( m_socket->bytesAvailable() < 8 )
    {
        if( !m_socket->waitForReadyRead(3 * 1000))
        {
            LOG_INFO("BulkDownload session timed out");
            return;
        }
    }
    
    QDataStream in(m_socket);
    in >> maxBytes >> maxRate;
    
    // Send data
    const int packetSize = 1000;
    qint32 bytesSent = 0;
    QByteArray block(packetSize, 0);
    QTime t;
    t.start();

    while( maxBytes == -1 || bytesSent < maxBytes )
    {
        m_socket->write(block);
        m_socket->waitForBytesWritten(-1);
        
        if( m_socket->state() == QAbstractSocket::UnconnectedState )
        {
            break;
        }
        
        LOG_DEBUG("BulkDownloadServerSession %d / %d", 
            (int)bytesSent, (int)maxBytes);
        
        // Rate limit
        if( maxRate != -1 && bytesSent / maxRate * 8000 > t.elapsed())
        {
            msleep(qMax(bytesSent / maxRate * 8000 - t.elapsed(), 0));
        }
    }

    // Connection closed by client
    if( m_socket->state() == QAbstractSocket::UnconnectedState )
    {
        // nothing to do here
    }
    else // all bytes sent
    {
        // Close connection
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected();
    }
    
    m_socket->close();
    LOG_DEBUG("End of BulkDownloadServerSession::run");
}
