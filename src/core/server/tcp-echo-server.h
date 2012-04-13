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

#ifndef TCP_ECHO_SERVER_H
#define TCP_ECHO_SERVER_H

#include "tcp-server.h"

/**
 * \brief The tcp echo server session
 * \ingroup Servers
 */
class TcpEchoServerSession : public TcpServerSession
{
public:
    /**
     * \brief Initialize the tcp echo server session
     * \param socket The socket for the session
     */
    TcpEchoServerSession(int socketDescriptor);
    virtual void run();
};

/**
 * \brief The tcp echo server
 * \ingroup Servers
 */
class TcpEchoServer : public TcpServer
{
public:
    /**
     * \brief Initialize the tcp echo server
     * \param addr The ip address of the server
     * \param port The port of the server
     */
    TcpEchoServer(const QHostAddress &addr, quint16 port);
    virtual TcpServerSession *createSession(int socketDescriptor);
};

#endif /* TCP_ECHO_SERVER_H */

