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

#include "console-application.h"

bool ConsoleApplication::parseAddr(const QString &str, QHostAddress &addr)
{
    QRegExp rx("^(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)\\."
               "(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)$");
    if( rx.indexIn(str) == -1 )
    {
        LOG_ERROR(QString("Invalid server daemon address %1").arg(str));
        return false;
    }
    if( rx.cap(1).toUInt() > 255 || 
        rx.cap(2).toUInt() > 255 ||
        rx.cap(3).toUInt() > 255 ||
        rx.cap(4).toUInt() > 255 )
    {
        LOG_ERROR(QString("Invalid server daemon address %1").arg(str));
        return false;
    }
    
    addr = QHostAddress(str);
    
    return true;
}

bool ConsoleApplication::parsePort(const QString &str, quint16 &port)
{
    bool ok;
    port = str.toUShort(&ok);
    if( !ok )
    {
        LOG_ERROR(QString("Invalid server daemon port %1").arg(str));
        return false;
    }
    else if( port < 1024 )
    {
        LOG_ERROR("Server daemon port's range is 1024 ~ 65535");
        return false;
    }
    return true;
}

bool ConsoleApplication::parseAddrRange(const QString &str, 
                                        QList<QHostAddress> &addrRange)
{
    QRegExp rx("^(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)\\."
               "(0|[1-9][0-9]*)\\.(0|[1-9][0-9]*)/(0|[1-9][0-9]*)$");
    if( rx.indexIn(str) == -1 )
    {
        LOG_ERROR(QString("Invalid server address range %1").arg(str));
        return false;
    }
    if( rx.cap(1).toUInt() > 255 || 
        rx.cap(2).toUInt() > 255 ||
        rx.cap(3).toUInt() > 255 ||
        rx.cap(4).toUInt() > 255 )
    {
        LOG_ERROR(QString("Invalid server address range %1").arg(str));
        return false;
    }
    if( rx.cap(5).toInt() < 26 || 
        rx.cap(5).toInt() > 29 )
    {
        LOG_ERROR("Subnet mask should be /26, /27, /28 or /29");
        return false;
    }
    
    quint32 mask = 0xffffffff << (32 - rx.cap(5).toUInt());
    int n = 1 << (32 - rx.cap(5).toUInt());
    quint32 firstAddr = (
        (rx.cap(1).toUInt() << 24) | 
        (rx.cap(2).toUInt() << 16) | 
        (rx.cap(3).toUInt() << 8) | 
        (rx.cap(4).toUInt())) & mask;

    for(int i = 0; i < n; i++)
    {
        addrRange.append(QHostAddress(firstAddr + i));
    }
    return true;
}

ConsoleApplication::ConsoleApplication(int argc, char *argv[])
{
    LOG_DEBUG("Beginning of ConsoleApplication::ConsoleApplicaiton");
    
    // Check arguments and init variables
    if( argc != 4 )
    {
        printf(
            "algts console application\n"
            "-------------------------\n"
            "Usage: \n"
            "    app <local-address-range> <daemon-address> <daemon-port>\n"
            "Example: \n"
            "    sample 172.16.0.8/29 10.0.0.1 3200\n");
        exit(1);
    }
    
    QList<QHostAddress> specifiedLocalAddrs;
    
    if( !parseAddrRange(argv[1], specifiedLocalAddrs) ||
        !parseAddr(argv[2], m_serverDaemonAddr) ||
        !parsePort(argv[3], m_serverDaemonPort))
    {
        exit(1);
    }
    
    // Add the specified local addresses that are actually available 
    // to m_localAddrs
    
    
    QList<QHostAddress> allAddrs = QNetworkInterface::allAddresses();
    
    for(int i = 0; i < specifiedLocalAddrs.size(); i++)
    {
        if( allAddrs.contains(specifiedLocalAddrs[i]))
        {
            LOG_INFO(QString("Address %1 available")
                .arg(specifiedLocalAddrs[i].toString()));
            
            m_localAddrs.append(specifiedLocalAddrs[i]);
        }
        else
        {
            LOG_INFO(QString("Address %1 not available")
                .arg(specifiedLocalAddrs[i].toString()));
        }
    }
    LOG_DEBUG("End of ConsoleApplication::ConsoleApplicaiton");
}

