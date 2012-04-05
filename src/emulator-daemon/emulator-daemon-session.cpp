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

#include "emulator-daemon-session.h"
#include "netem-session.h"

EmulatorDaemonSession::EmulatorDaemonSession(QTcpSocket *socket, QMap<QString, QString> &params)
    : m_socket(socket), m_params(params)
{
}

void EmulatorDaemonSession::run()
{
    LOG_DEBUG("Beginning of EmulatorDaemonSession::run");

    QDataStream in(m_socket);
    
    // Receive the whole packet
    while( m_socket->bytesAvailable() < 4) 
    {
        if (!m_socket->waitForReadyRead(3 * 1000)) 
        {
            LOG_INFO("Emulator daemon session timed out");
            return;
        }
    }
    
    quint32 blockSize;
    in >> blockSize;
    
    while( m_socket->bytesAvailable() < blockSize ) 
    {
        if (!m_socket->waitForReadyRead(3 * 1000)) 
        {
            LOG_INFO("Emulator daemon session timed out");
            return;
        }
    }

    // Check name
    QString emulatorName;
    in >> emulatorName;
    
    if( emulatorName == "NetEm" )
    {
        // Continue parsing emulator specific parameters, and
        // setup emulation environment
        NetemSession ns;
        ns.parse(m_socket, m_params);
    }
    else
    {
        LOG_INFO(QString("Emulator %1 not supported").arg(emulatorName));
    }
    LOG_DEBUG("End of EmulatorDaemonSession::run");
}
