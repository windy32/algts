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

#include "tcp-server.h"
#include "../QTcpServerEx.h"

TcpServer::TcpServer(const QHostAddress &addr, quint16 port)
    : Server(addr, port), m_ready(false)
{
}

void TcpServer::run()
{
    LOG_DEBUG("Beginning of TcpServer::run");
    
    // Initialize the server
    QTcpServerEx server;
    if( server.listen(QHostAddress(m_addr), m_port))
    {
        m_result = true;
        m_description = QString("Server started @ %1:%2")
            .arg(m_addr.toString()).arg(m_port);
        LOG_INFO(m_description);
        m_ready = true;
    }
    else
    {
        m_result = false;
        m_description = QString("Cannot start server @ %1:%2")
            .arg(m_addr.toString()).arg(m_port);
        LOG_ERROR(m_description);
        m_ready = true;
    }

    QList<TcpServerSession *> threads;

    // Process incoming requests
    while( true )
    {
        // Wait for a new connection
        if( !server.hasPendingDescriptor())
        {
            server.waitForNewConnection(-1);
        }
        int socketDescriptor = server.nextPendingDescriptor();
        
        // Start a new thread for the session
        TcpServerSession *session = createSession(socketDescriptor);
        threads.append(session);
        //connect(session, SIGNAL(finished()), this, SLOT(deleteLator()));
        session->start();
        
        // Free Memory
        for (int i = threads.size() - 1; i >= 0; i--)
        {
            if (threads[i]->isFinished())
            {
                delete threads[i];
                threads.removeAt(i);
            }
        }
    }

    // TODO: capture ctrl+c and stop the loop above
    server.close();
    LOG_DEBUG("End of TcpServer::run");
}

bool TcpServer::start(QString &description)
{
    // Start the thread
    QThread::start();
    
    // Wait until initialization is finished
    while( !m_ready )
    {
        msleep(10);
    }
    
    description = m_description;
    return m_result;
}

TcpServerSession::TcpServerSession(int socketDescriptor)
    : m_socketDescriptor(socketDescriptor)
{
}

