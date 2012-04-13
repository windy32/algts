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

#include "client-factory.h"

#include "../client/bulk-download-client.h"
#include "../client/bulk-upload-client.h"
#include "../client/onoff-download-client.h"
#include "../client/tcp-echo-client.h"
#include "../client/async-udp-echo-client.h"

Client *ClientFactory::create(Task::Type type, Task *task, 
    const QString &localAddr, 
    const QString &serverAddr)
{
    if( type == Task::BULK_DOWNLOAD )
    {
        return new BulkDownloadClient(localAddr, serverAddr, 
                                     (BulkDownloadTask *)task);
    }
    else if( type == Task::BULK_UPLOAD )
    {
        return new BulkUploadClient(localAddr, serverAddr, 
                                   (BulkUploadTask *)task);
    }
    else if( type == Task::ONOFF_DOWNLOAD )
    {
        return new OnoffDownloadClient(localAddr, serverAddr, 
                                      (OnoffDownloadTask *)task);
    }
    else if( type == Task::TCP_ECHO )
    {
        return new TcpEchoClient(localAddr, serverAddr, 
                                (TcpEchoTask *)task);
    }
    else if( type == Task::ASYNC_UDP_ECHO )
    {
        return new AsyncUdpEchoClient(localAddr, serverAddr, 
                                     (AsyncUdpEchoTask *)task);
    }
    else
    {
        LOG_ERROR("Invalid task type");
        return NULL;
    }
}

