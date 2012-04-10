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

#ifndef ASYNC_UDP_ECHO_SERVER_H
#define ASYNC_UDP_ECHO_SERVER_H

#include "server.h"

/**
 * \brief The asynchronous udp echo server
 * \ingroup Servers
 */
class AsyncUdpEchoServer : public Server
{
private:
    QUdpSocket m_socket;
    
protected:
    virtual void run();

public:
    /**
     * \brief Initialize the asynchronous udp echo server
     * \param addr The ip address of the server
     * \param port The port of the server
     */
    AsyncUdpEchoServer(const QHostAddress &addr, quint16 port);
	virtual bool start(QString &description);
};

#endif /* ASYNC_UDP_ECHO_SERVER_H */

