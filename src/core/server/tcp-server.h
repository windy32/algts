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

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "server.h"

/**
 * \brief The base class of all tcp servers sessions
 */
class TcpServerSession : public QThread
{
protected:
    /**
     * \brief The socket descriptor for the session
     */
    int m_socketDescriptor;

protected:
    /**
     * \brief The entry point of the thread function
     * \note To start a thread in Qt, exec() is the right one to call
     */
    virtual void run() = 0;
    
public:
    /**
     * \brief Initialize the tcp server session
     * \param socketDescriptor The socket descriptor for the session
     */
    TcpServerSession(int socketDescriptor);
};

/**
 * \brief The base class of all tcp servers
 * \ingroup Servers
 */
class TcpServer : public Server
{
private:
    bool m_result; // Result of the initialization
    QString m_description; // Detailed info of initialization
    bool m_ready; // If initialization is finished
    
protected:
    virtual void run();

public:
    /**
     * \brief Initialize the tcp server
     * \param addr The ip address of the server
     * \param port The port of the server
     */
    TcpServer(const QHostAddress &addr, quint16 port);
    
    /**
     * \brief Create a session for the new connection
     * \param socketDescriptor The socket descriptor for the new connection
     * \return A pointer to the session object that will process client's 
     *         requests
     */
    virtual TcpServerSession *createSession(int socketDescriptor) = 0;
    
    virtual bool start(QString &description);
};

#endif /* TCP_SERVER_H */

