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

#include "netem-emulator.h"

NetemEmulator::NetemEmulator(const QString &addr, quint16 port)
    : Emulator(addr, port)
{
}

void NetemEmulator::commit()
{
    LOG_DEBUG("Beginning of NetemEmulator::commit");

    // Connect to emulator daemon
    QTcpSocket socket;
    socket.connectToHost(QHostAddress(m_addr), m_port);
    if( !socket.waitForConnected(3 * 1000))
    {
        LOG_ERROR(QString("Cannot connect to emulator daemon @ %1:%2")
            .arg(m_addr).arg(m_port));
        return;
    }
    
    // Send request
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)0;
    out << QString("NetEm");
    out << QString("COMMIT");
    out << m_params;
    out.device()->seek(0);
    out << (quint32)(block.size() - 4);
    
    socket.write(block);
    socket.waitForBytesWritten(-1);
    
    // Receive response
    while( socket.bytesAvailable() < 4 )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_WARN("NetemEmulator commit timed out");
            return;
        }
    }
    
    quint32 blockSize;
    QDataStream in(&socket);
    in >> blockSize;
    
    while( socket.bytesAvailable() < blockSize )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_WARN("NetemEmulator commit timed out");
            return;
        }
    }
    
    bool ok;
    QString description;
    in >> ok >> description;
    
    // Display result
    if( ok )
    {
        LOG_INFO("Parameters successfully committed");
        LOG_INFO(QString("Description: %1").arg(description));
    }
    else
    {
        LOG_WARN("Parameter commitment failed");
        LOG_WARN(QString("Description: %s").arg(description));
    }

    // Close connection
    socket.disconnectFromHost();
    socket.waitForDisconnected();

    LOG_DEBUG("End of NetemEmulator::commit");
}

void NetemEmulator::reset()
{
    LOG_DEBUG("End of NetemEmulator::reset");

    // Connect to emulator daemon
    QTcpSocket socket;
    socket.connectToHost(QHostAddress(m_addr), m_port);
    if( !socket.waitForConnected(3 * 1000))
    {
        LOG_ERROR(QString("Cannot connect to emulator daemon @ %1:%2")
            .arg(m_addr).arg(m_port));
        return;
    }
    
    // Send request
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)0;
    out << QString("NetEm");
    out << QString("RESET");
    out.device()->seek(0);
    out << (quint32)(block.size() - 4);

    socket.write(block);
    socket.waitForBytesWritten(-1);
    
    // Receive response
    while( socket.bytesAvailable() < 4 )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_WARN("NetemEmulator reset timed out");
            return;
        }
    }
    
    quint32 blockSize;
    QDataStream in(&socket);
    in >> blockSize;
    
    while( socket.bytesAvailable() < blockSize )
    {
        if( !socket.waitForReadyRead(3 * 1000))
        {
            LOG_WARN("NetemEmulator reset timed out");
            return;
        }
    }
    
    bool ok;
    QString description;
    in >> ok >> description;
    
    // Display result
    if( ok )
    {
        LOG_INFO("Parameters successfully reset");
        LOG_INFO(QString("Description: %1").arg(description));
        
        m_params.clear();
    }
    else
    {
        LOG_WARN("Parameter reset failed");
        LOG_WARN(QString("Description: %1").arg(description));
    }

    // Close connection
    socket.disconnectFromHost();
    socket.waitForDisconnected();

    LOG_DEBUG("End of NetemEmulator::reset");
}
