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

#ifndef BULK_DOWNLOAD_SERVER_H
#define BULK_DOWNLOAD_SERVER_H

#include "tcp-server.h"

class BulkDownloadServer : public TcpServer
{
public:
    BulkDownloadServer(const QHostAddress &addr, quint32 port);
    virtual TcpServerSession *createSession(QTcpSocket *socket);
};

class BulkDownloadServerSession : public TcpServerSession
{
public:
    BulkDownloadServerSession(QTcpSocket *socket);
    virtual void run();
};

#endif /* BULK_DONWLOAD_SERVER_H */

