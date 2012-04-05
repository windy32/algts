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

TcpServerSession *TcpEchoServer::createSession(QTcpSocket *socket)
{
    return new TcpEchoServerSession(socket);
}

TcpEchoServerSession::TcpEchoServerSession(QTcpSocket *socket)
    : TcpServerSession(socket)
{
}

void TcpEchoServerSession::run()
{
    LOG_DEBUG("Beginning of TcpEchoServerSession::run");

    while( true )
    {
        // Receive request
        qint16 fillSize;
        qint16 echoSize;

        while( m_socket->bytesAvailable() < 4 )
        {
            if( !m_socket->waitForReadyRead(3 * 1000))
            {
                LOG_INFO("TcpEcho session timed out");
                return;
            }
        }

        QDataStream in(m_socket);
        in >> fillSize >> echoSize;

        // Receive filled bytes in the request
        while( m_socket->bytesAvailable() < fillSize )
        {
            if( !m_socket->waitForReadyRead(3 * 1000))
            {
                LOG_INFO("TcpEcho session timed out");
                return;
            }
        }
        
        // Send data
        QByteArray block(echoSize, 0);
        m_socket->write(block);
        m_socket->waitForBytesWritten(-1);

        if( m_socket->state() == QAbstractSocket::UnconnectedState )
        {
            break;
        }
        LOG_DEBUG("TcpEchoSession sent an echo of %d bytes", (int)echoSize);
    }
    
    LOG_DEBUG("End of TcpEchoServerSession::run");
}
