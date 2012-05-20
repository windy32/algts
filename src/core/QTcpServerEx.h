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

#ifndef Q_TCP_SERVER_EX_H
#define Q_TCP_SERVER_EX_H

#include <QTcpServer>
#include <QQueue>
#include <QMutex>

/**
 * \ingroup Constructs
 * \brief Multi-thread support for tcp servers
 *
 * The returned QTcpSocket object in QTcpSocket::nextPendingConnection cannot be
 * used from another thread. It's necessary to override incomingConnection, 
 * if the incoming connection shall be used in another thread. 
 */

class QTcpServerEx : public QTcpServer
{
private:
    QMutex m_mutex;

protected:
    /**
     * \brief The list of pending connections as socket descriptors
     */
    QQueue<int> m_pendingDescriptors;
    
protected:
    /**
     * \brief Stores the new socket descriptor in an internal list
     */
    void incomingConnection(int socketDescriptor);

public:
    /**
     * \brief Returns the next pending connection as a socket descriptor
     */
    int nextPendingDescriptor();

    /**
     * \brief Returns true if there is a pending socket descriptor, and false
     *        otherwise
     */
    bool hasPendingDescriptor();
};

#endif /* Q_TCP_SERVER_EX_H */

