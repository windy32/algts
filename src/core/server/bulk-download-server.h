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

#ifndef BULK_DOWNLOAD_SERVER_H
#define BULK_DOWNLOAD_SERVER_H

#include "tcp-server.h"

/**
 * \brief The bulk download server
 * \ingroup Servers
 */
class BulkDownloadServerSession : public TcpServerSession
{
public:
    /**
     * \brief Initialize the bulk download server session
     * \param socketDescriptor The socket descriptor for the session
     */
    BulkDownloadServerSession(int socketDescriptor);
    virtual void run();
};

/**
 * \brief The bulk download server session
 * \ingroup Servers
 */
class BulkDownloadServer : public TcpServer
{
public:
    /**
     * \brief Initialize the bulk download server
     * \param addr The ip address of the server
     * \param port The port of the server
     */
    BulkDownloadServer(const QHostAddress &addr, quint32 port);
    virtual TcpServerSession *createSession(int socketDescriptor);
};

#endif /* BULK_DONWLOAD_SERVER_H */

