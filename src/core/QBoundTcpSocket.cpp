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

#include "QBoundTcpSocket.h"

#ifdef Q_OS_LINUX
    #include <arpa/inet.h>
#endif

bool QBoundTcpSocket::bindAndConnect(const QString &localAddr, quint16 localPort, 
                                     const QString &serverAddr, quint16 serverPort)
{
#ifdef Q_OS_LINUX
    LOG_DEBUG("Beginning of QBoundTcpSocket::bindAndConnect");

    int sockfd;
    int result;
    struct sockaddr_in localSockAddr;
    struct sockaddr_in serverSockAddr;
    bzero(&localSockAddr, sizeof(localSockAddr));
    bzero(&serverSockAddr, sizeof(serverSockAddr));
    
    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd == -1 ) 
    {
        qDebug() << "QBoundTcpSocket: Cannot create socket";
        return false;
    }
    
    // bind
    localSockAddr.sin_family = AF_INET;
    localSockAddr.sin_port = htons(localPort);
    localSockAddr.sin_addr.s_addr = inet_addr(localAddr.toLocal8Bit().data());

    result = bind(sockfd, (struct sockaddr *)&localSockAddr, sizeof(localSockAddr));
    if( result == -1 ) 
    {
        qDebug() << "QBoundTcpSocket: Cannot bind socket";
        return false;
    }
    
    // connect
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(serverPort);
    serverSockAddr.sin_addr.s_addr = inet_addr(serverAddr.toLocal8Bit().data());

    result = ::connect(sockfd, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr));
    if( result == -1 ) 
    {
        qDebug() << "QBoundTcpSocket: Cannot connect to server";
        return false;
    }
    
    // set socket descriptor
    if( !setSocketDescriptor(sockfd, QAbstractSocket::ConnectedState))
    {
        qDebug() << "QBoundTcpSocket: Cannot set socket descriptor";
        return false;
    }
    
    LOG_DEBUG("End of QBoundTcpSocket::bindAndConnect");
    return true;
#else
    qDebug() << "QBoundTcpSocket for Windows/Mac OS is not implemented yet";
    return false;
#endif
}
