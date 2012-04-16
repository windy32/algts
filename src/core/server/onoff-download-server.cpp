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

#include "onoff-download-server.h"

OnoffDownloadServer::OnoffDownloadServer(const QHostAddress &addr, quint16 port)
    : TcpServer(addr, port)
{
}

TcpServerSession *OnoffDownloadServer::createSession(int socketDescriptor)
{
    return new OnoffDownloadServerSession(socketDescriptor);
}

OnoffDownloadServerSession::OnoffDownloadServerSession(int socketDescriptor)
    : TcpServerSession(socketDescriptor)
{
}

void OnoffDownloadServerSession::run()
{
    LOG_INFO("Beginning of OnoffDownloadServerSession::run");
    
    QTcpSocket socket;
    if( !socket.setSocketDescriptor(m_socketDescriptor))
    {
        LOG_ERROR("Cannot set socket descriptor");
    }

    // Receive request
    qint16 fillSize;
    qint32 onTime;
    qint32 maxRate;
    qint16 packetSize;
    
    while( socket.bytesAvailable() < 12 )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_INFO("OnoffDownload session timed out");
            return;
        }
    }

    QDataStream in(&socket);
    in >> fillSize >> onTime >> maxRate >> packetSize;
    
    // Receive filled bytes in the request
    while( socket.bytesAvailable() < fillSize )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_INFO("OnoffDownload session timed out");
            return;
        }
    }
    
    // Send data
    qint32 totalBytes = 0;
    QByteArray block(packetSize, 0);
    QTime t;
    t.start();

    while( t.elapsed() < onTime )
    {
        qint32 bytesSent = (qint32)socket.write(block);
        totalBytes += bytesSent;
        socket.waitForBytesWritten(-1);

        if( socket.state() == QAbstractSocket::UnconnectedState )
        {
            break;
        }
        
        LOG_DEBUG("OnoffDownloadServerSession %d / %d", 
            (int)bytesSent, (int)totalBytes);
        
        // Rate limit
        if( maxRate != -1 && totalBytes * 1000 / maxRate > t.elapsed())
        {
            msleep(qMax(totalBytes * 1000 / maxRate - t.elapsed(), 0));
        }
    }

    // Close connection
    socket.close();
    LOG_INFO("End of OnoffDownloadServerSession::run");
}
