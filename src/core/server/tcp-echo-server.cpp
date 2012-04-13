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

#include "tcp-echo-server.h"

TcpEchoServer::TcpEchoServer(const QHostAddress &addr, quint16 port)
    : TcpServer(addr, port)
{
}

TcpServerSession *TcpEchoServer::createSession(int socketDescriptor)
{
    return new TcpEchoServerSession(socketDescriptor);
}

TcpEchoServerSession::TcpEchoServerSession(int socketDescriptor)
    : TcpServerSession(socketDescriptor)
{
}

void TcpEchoServerSession::run()
{
    LOG_DEBUG("Beginning of TcpEchoServerSession::run");

    QTcpSocket socket;
    if( !socket.setSocketDescriptor(m_socketDescriptor))
    {
        LOG_ERROR("Cannot set socket descriptor");
    }

    while( true )
    {
        // Receive request
        qint16 fillSize;
        qint16 echoSize;

        while( socket.bytesAvailable() < 4 )
        {
            if( !socket.waitForReadyRead(3 * 1000))
            {
                LOG_INFO("TcpEcho session timed out");
                return;
            }
        }

        QDataStream in(&socket);
        in >> fillSize >> echoSize;

        // Receive filled bytes in the request
        while( socket.bytesAvailable() < fillSize )
        {
            if( !socket.waitForReadyRead(3 * 1000))
            {
                LOG_INFO("TcpEcho session timed out");
                return;
            }
        }
        
        // Send data
        QByteArray block(echoSize, 0);
        socket.write(block);
        socket.waitForBytesWritten(-1);

        if( socket.state() == QAbstractSocket::UnconnectedState )
        {
            break;
        }
        LOG_DEBUG("TcpEchoSession sent an echo of %d bytes", (int)echoSize);
    }
    
    socket.close();
    LOG_DEBUG("End of TcpEchoServerSession::run");
}

