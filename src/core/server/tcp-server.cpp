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

#include "tcp-server.h"

TcpServer::TcpServer(const QHostAddress &addr, quint16 port)
    : Server(addr, port)
{
}

void TcpServer::run()
{
    LOG_DEBUG("Beginning of TcpServer::run");

    while( true )
    {
        // Wait for a new connection
        m_server.waitForNewConnection(-1);
        QTcpSocket *socket = m_server.nextPendingConnection();
        LOG_INFO("A new connection established");
        
        // Start a new thread for the session
        TcpServerSession *session = createSession(socket);
        connect(session, SIGNAL(finished()), this, SLOT(deleteLator()));
        session->start();
    }

    // TODO: capture ctrl+c and stop the loop above
    m_server.close();
    LOG_DEBUG("End of TcpServer::run");
}

bool TcpServer::start(QString &description)
{
    if( m_server.listen(QHostAddress(m_addr), m_port))
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

TcpServerSession::TcpServerSession(QTcpSocket *socket)
    : m_socket(socket)
{
}
