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

#include "server-factory.h"

#include "bulk-download-server.h"
#include "bulk-upload-server.h"
#include "onoff-download-server.h"
#include "tcp-echo-server.h"
#include "async-udp-echo-server.h"

Server *ServerFactory::create(Task::Type type, const QHostAddress &addr, 
    quint16 port)
{
    if( type == Task::BULK_DOWNLOAD )
    {
        return new BulkDownloadServer(addr, port);
    }
    else if( type == Task::BULK_UPLOAD )
    {
        return new BulkUploadServer(addr, port);
    }
    else if( type == Task::ONOFF_DOWNLOAD )
    {
        return new OnoffDownloadServer(addr, port);
    }
    else if( type == Task::TCP_ECHO )
    {
        return new TcpEchoServer(addr, port);
    }
    else if( type == Task::ASYNC_UDP_ECHO )
    {
        return new AsyncUdpEchoServer(addr, port);
    }
    else
    {
        LOG_ERROR("Invalid task type");
        return NULL;
    }
}
