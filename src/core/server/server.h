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

#ifndef SERVER_H
#define SERVER_H

#include "../common.h"

/**
 * \defgroup Servers Servers
 * \ingroup Core
 */

/**
 * \brief The base class of all servers
 * \ingroup Servers
 */
class Server : public QThread
{
protected:
    /**
     * \brief Server's ip address
     */
    QHostAddress m_addr;
    
    /**
     * \brief Server's port
     */
    quint16 m_port;

protected:
    /**
     * \brief The entry point of the thread function
     * \note To start a thread in Qt, exec() is the right one to call
     */
    virtual void run() = 0;
    
public:
    /**
     * \brief Initialize the base class with specified ip address and port
     * \param addr The ip address of the server
     * \param port The port of the server
     */
    Server(const QHostAddress &addr, quint16 port);
    
    /**
     * \brief Start the server
     * \param description Detailed description of the result
     * \return Returns true if server started successfully (which means that
     *         calls to bind, listen, etc. are successful), and false otherwise
     * \note The function will return quickly, as another thread will be created
     *       waiting for clients' requests
     */
    virtual bool start(QString &description) = 0;
};

#endif /* SERVER_H */

