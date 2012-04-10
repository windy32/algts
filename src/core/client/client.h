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

#ifndef CLIENT_H
#define CLIENT_H

#include "../common.h"

#include "../log.h"
#include "../trace.h"
#include "../global-timer.h"

#include "../QBoundTcpSocket.h"

/**
 * \defgroup Clients Clients
 * \ingroup Core
 */

/**
 * \brief The base class of all clients
 * \ingroup Clients
 */
class Client : public QThread
{
protected:
    /**
     * \brief Server's IPv4 address
     */
    QString m_serverAddr;
    
    /**
     * \brief Client's IPv4 address
     */
    QString m_localAddr;

protected:
    /**
     * \brief The entry point of the thread function
     * \note To start a thread in Qt, exec() is the right one to call
     */
	virtual void run() = 0;

public:
    /**
     * \brief Initialize the base class with specific local address and server
     *        address
     * \param localAddr Client's IPv4 address
     * \param serverAddr Server's IPv4 address
     */
    Client(const QString &localAddr, const QString &serverAddr);
    
    /**
     * \brief Generate text trace
     * \param trace The text trace item for output
     */
    virtual void generateTextTrace(TextTraceItem &trace) = 0;
    
    /**
     * \brief Generate regular trace
     * \param trace The regular trace item for output
     * \param length The length of the regular trace
     */
    virtual void generateRegularTrace(RegularTraceItem &trace, int length) = 0;
};

#endif /* CLIENT_H */

