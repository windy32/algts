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

#include "async-udp-echo-server.h"

AsyncUdpEchoServer::AsyncUdpEchoServer(const QHostAddress &addr, quint16 port)
    : Server(addr, port)
{
}

void AsyncUdpEchoServer::run()
{
    LOG_DEBUG("Beginning of AsyncUdpEchoServer::run");

    while( true )
    {
        if( m_socket.waitForReadyRead(3 * 1000))
        {
            // Receive input
            QHostAddress clientAddr;
            quint16 clientPort;
            char buffer[1500];
            qint16 echoSize;

            qint64 bytesRead = m_socket.readDatagram(buffer, 1500, &clientAddr, &clientPort);
            if( bytesRead < 2 )
            {
                LOG_INFO("Corrupt packet received");
                continue;
            }
            QDataStream in(QByteArray(buffer, 1500));
            in >> echoSize;
            
            // Send echo
            qint64 bytesWritten = m_socket.writeDatagram(
                QByteArray(echoSize, 0), clientAddr, clientPort);
            if( bytesWritten == -1 )
            {
                LOG_INFO(QString("Cannot send echo packet to client %1:%2")
                    .arg(clientAddr.toString()).arg(clientPort));
                continue;
            }
            else if( bytesWritten == 0 )
            {
                LOG_DEBUG(QString("No bytes sent to client %1:%2")
                    .arg(clientAddr.toString()).arg(clientPort));
                continue;
            }
        }
    }

    // TODO: capture ctrl+c and stop the loop above
    LOG_DEBUG("End of AsyncUdpEchoServer::run");
}

bool AsyncUdpEchoServer::start(QString &description)
{
    // Bind local address
    if( m_socket.bind(QHostAddress(m_addr), m_port))
    {
        description = QString("Server started @ %1:%2")
            .arg(m_addr.toString()).arg(m_port);
        LOG_INFO(description);
        QThread::start();
        return true;
    }
    else
    {
        description = QString("Cannot start server @ %1:%2")
            .arg(m_addr.toString()).arg(m_port);
        LOG_ERROR(description);
        return false;
    }
}
