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

#include "bulk-upload-server.h"

BulkUploadServer::BulkUploadServer(const QHostAddress &addr, quint32 port)
    : TcpServer(addr, port)
{
}

TcpServerSession *BulkUploadServer::createSession(int socketDescriptor)
{
    return new BulkUploadServerSession(socketDescriptor);
}

BulkUploadServerSession::BulkUploadServerSession(int socketDescriptor)
    : TcpServerSession(socketDescriptor)
{
}

void BulkUploadServerSession::run()
{
    LOG_INFO("Beginning of BulkUploadServerSession::run");
    
    QTcpSocket socket;
    if( !socket.setSocketDescriptor(m_socketDescriptor))
    {
        LOG_ERROR("Cannot set socket descriptor");
    }

    char buffer[256 * 1024];
    qint32 totalBytes = 0;
    
    // Receive data
    while( socket.waitForReadyRead(5 * 1000))
    {
        qint64 bytesRead = socket.read(buffer, 256 * 1024);
        totalBytes += bytesRead;
        
        LOG_DEBUG("BulkUploadServerSession %d / %d", 
            (int)bytesRead, (int)totalBytes);
    }
    
    // Connection closed by client
    socket.close();
    LOG_INFO("End of BulkUploadServerSession::run");
}
