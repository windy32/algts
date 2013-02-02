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

#include "../log.h"
#include "../scenario.h"
#include "../client/client-factory.h"

/**
 * \defgroup Applications Applications
 * \ingroup Core
 */

/**
 * \ingroup Applications
 * \brief The core application.
 *
 * A scenario mainly consists of a number of users and different types of tasks,
 * and a task consists of a server port number, a start time, stop time, and
 * some concrete-task-specific attributes.
 *
 * However the information provided by a scenario is not enough at execution,
 * as each user should have a unique ip address, and clients should connect to
 * a specific ip address that corresponds to the type of the task.
 *
 * Thus the CoreApplication is designed for holding the extra information as
 * well as execuing the tasks in a scenario.
 */
class CoreApplication
{
protected:
    /**
     * \brief Local addresses that can be used for clients
     */
    QList<QHostAddress> m_localAddrs;
    /**
     * \brief Address of the server daemon
     */
    QHostAddress m_serverDaemonAddr;
    /**
     * \brief Port of the server daemon
     */
    quint16 m_serverDaemonPort;

    /**
     * \brief Raw trace generated while executing tasks
     *
     * This is a rather complicated container structure.
     *  - 1st index is username
     *  - 2nd index is the task index
     *  - 3rd index is the name of the trace item (within RawTraceItem)
     *  - 4th index is the data index of the trace item (within RawTraceItem)
     */
    QMap<QString, QList<RawTraceItem> > m_rawTrace;
public:
    /**
     * \brief Constructor that does nothing
     * \note This function should only be used in CoreApplication's derived
     *       classes
     * \see ConsoleApplication
     */
    CoreApplication();
    /**
     * \brief Initialize the core application
     * \param localAddrs Local addresses that can be used for clients
     * \param serverDaemonAddr Address of the server daemon
     * \param serverDaemonPort Port of the server daemon
     * \note The localAddrs should be the addresses actually available on the
     *       local host, the constructor does not automatically filter addresses
     *       which are not available.
     */
    CoreApplication(const QList<QHostAddress> &localAddrs,
                    const QHostAddress &serverDaemonAddr,
                    quint16 serverDaemonPort);

    /**
     * \brief Execute tasks in the scenario
     * \param s Pointer to the scenario
     * 
     * The execution follows the procedure below.
     *  1. Exit if no enough local addresses are available
     *  2. Create the mapping from usernames to local addresses
     *  3. Connect to the server daemon and send request
     *     Field        | Type         | Description
     *     -------------|--------------|---------------------------
     *     TaskType     | int32        | Type of the server
     *     Port         | uint16       | Port of the server
     *  4. Get response from the server daemon.
     *
     *     When the server's set up successfully, the response would be
     *     Field        | Type         | Description
     *     -------------|--------------|---------------------------
     *     BlockSize    | uint32       | Size of the remaining part
     *     Result       | bool         | true
     *     Address      | quint32      | IPv4 Address of the server
     *     Description  | QString      | Detailed description
     *     Otherwise, the response wound be
     *     Field        | Type         | Description
     *     -------------|--------------|---------------------------
     *     BlockSize    | uint32       | Size of the remaining part
     *     Result       | bool         | false
     *     Description  | QString      | Detailed description
     *  5. Exit if any error occurrs
     *  6. Create client objects with generated server addresses and local
     *     addresses
     *  7. Execute client objects
     *  8. Wait until thread exits
     *  9. Generate trace file if desired
     */
    void exec(Scenario *s);
    void exportRawTrace(QString userName, int taskIndex, RawTraceItem &trace);
};

#endif /* CORE_APPLICATION_H */
