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

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "../common.h"
#include "../scenario.h"
#include "../client/client-factory.h"

/**
 * \brief The core application.
 *
 * ...detail
 */ 
class CoreApplication
{
protected:
    QList<QHostAddress> m_localAddrs;
    QHostAddress m_serverDaemonAddr;
    quint16 m_serverDaemonPort;
    
public:
    CoreApplication();
    CoreApplication(const QList<QHostAddress> &localAddrs,
                    const QHostAddress &serverDaemonAddr,
                    quint16 serverDaemonPort);

    // Execute tasks
    // 1. if no enough local addresses are available, exit
    // 2. create the mapping from user name to local address
    // 3. connect to server daemon and send layout [TaskType][Port] ...
    // 4. get response 
    //       [bool: Result][quint32: Address][QString: success description] or
    //       [bool: Result][QString: failure description]
    // 5. if any error occurred, tasks will no longer continue
    // 6. create client objects, with generated server addresses & local
    //    addresses
    // 7. execute client objects
    void  exec(Scenario *s);
};

#endif /* CORE_APPLICATION_H */
