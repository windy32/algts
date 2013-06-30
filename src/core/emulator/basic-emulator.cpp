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

#include "basic-emulator.h"

BasicEmulator::BasicEmulator(const QString &addr, quint16 port)
    : Emulator(addr, port)
{
}

void BasicEmulator::commit()
{
    LOG_DEBUG("Beginning of BasicEmulator::commit");

    // Connect to emulator daemon
    QTcpSocket socket;
    socket.connectToHost(QHostAddress(m_addr), m_port);
    if( !socket.waitForConnected(5 * 1000))
    {
        LOG_ERROR(QString("Cannot connect to emulator daemon @ %1:%2")
            .arg(m_addr).arg(m_port));
        return;
    }
    
    // Send request
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)0;
    out << QString("Basic");
    out << QString("COMMIT");
    out << m_params;
    out.device()->seek(0);
    out << (quint32)(block.size() - 4);
    
    socket.write(block);
    socket.waitForBytesWritten(-1);
    
    // Receive response
    while( socket.bytesAvailable() < 4 )
    {
        if( !socket.waitForReadyRead(5 * 1000))
        {
            LOG_WARN("BasicEmulator commit timed out");
            return;
        }
    }
    
    quint32 blockSize;
    QDataStream in(&socket);
    in >> blockSize;
    
    while( socket.bytesAvailable() < blockSize )
    {
        if( !socket.waitForReadyRead(5 * 1000))
        {
            LOG_WARN("BasicEmulator commit timed out");
            return;
        }
    }
    
    bool ok;
    QString description;
    in >> ok >> description;
    
    // Display result
    if( ok )
    {
        LOG_INFO(description);
    }
    else
    {
        LOG_WARN("Parameter commitment failed");
        LOG_WARN(QString("Description: %1").arg(description));
    }

    // Close connection
    socket.close();
    LOG_DEBUG("End of BasicEmulator::commit");
}

void BasicEmulator::reset()
{
    LOG_DEBUG("End of BasicEmulator::reset");

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
    out << QString("Basic");
    out << QString("RESET");
    out.device()->seek(0);
    out << (quint32)(block.size() - 4);

    socket.write(block);
    socket.waitForBytesWritten(-1);
    
    // Receive response
    while( socket.bytesAvailable() < 4 )
    {
        if( !socket.waitForReadyRead(5 * 1000))
        {
            LOG_WARN("BasicEmulator reset timed out");
            return;
        }
    }
    
    quint32 blockSize;
    QDataStream in(&socket);
    in >> blockSize;
    
    while( socket.bytesAvailable() < blockSize )
    {
        if( !socket.waitForReadyRead(5 * 1000))
        {
            LOG_WARN("BasicEmulator reset timed out");
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
    socket.close();
    LOG_DEBUG("End of BasicEmulator::reset");
}

