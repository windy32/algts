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

#include "core-application.h"

CoreApplication::CoreApplication()
    : m_serverDaemonPort(0)
{
}

CoreApplication::CoreApplication(
    const QList<QHostAddress> &localAddrs, 
    const QHostAddress &serverDaemonAddr, 
    quint16 serverDaemonPort)
    : m_localAddrs(localAddrs), 
      m_serverDaemonAddr(serverDaemonAddr),
      m_serverDaemonPort(serverDaemonPort)
{
}

void CoreApplication::exec(Scenario *s)
{
    // 1. if no enough local addresses are available, exit
    // 2. create the mapping from user name to local address
    // 3. expand tasks
    // 4. connect to server daemon and send layout [TaskType][Port] ...
    // 5. get response 
    //       [bool: Result][quint32: Address][QString: success description] or
    //       [bool: Result][QString: failure description]
    // 6. if any error occurred, tasks will no longer continue
    // 7. create client objects, with generated server addresses & local 
    //    addresses
    // 8. execute client objects
    // 9. wait until thread exits
    // 10. Generate trace file if desired
    
    // 1
    LOG_DEBUG("Checking for addresses available...");
    
    QMap<QString, QVector<Task *> > &tasks = s->tasks();
    if( m_localAddrs.size() < tasks.size())
    {
        LOG_ERROR("No enough addresses available");
        return;
    }
    
    // 2    
    QMap<QString, QHostAddress> addrTable;
    QMap<QString, QVector<Task *> >::iterator it;
    int index = 0;
    for(it = tasks.begin(), index = 0; it != tasks.end(); ++it, ++index)
    {
        addrTable[it.key()] = m_localAddrs[index];
        LOG_INFO(QString("%1's address: %2")
            .arg(it.key()).arg(m_localAddrs[index].toString())); 
    }
    
    // 3
    for(it = tasks.begin(); it != tasks.end(); ++it)
    {
        for(int i = 0; i < it.value().size(); i++)
        {
            it.value()[i]->expand();
            if( it.value()[i]->getStopTime() == -1 )
            {
                it.value()[i]->setStopTime(s->getLength() * 1000);
            }
        }
    }

    // 4, 5, 6, 7
    LOG_DEBUG("Setting up server daemon...");
    QList<Client *> clients;

    for(it = tasks.begin(); it != tasks.end(); ++it)
    {
        LOG_DEBUG(QString("User %1").arg(it.key()));
        
        for(int i = 0; i < it.value().size(); i++)
        {
            LOG_DEBUG("Task %d / %d", i + 1, it.value().size());
            
            Task *task = it.value()[i];
            
            // 4
            // Connect to server daemon
            QTcpSocket socket;
            socket.connectToHost(m_serverDaemonAddr, m_serverDaemonPort);
            if( !socket.waitForConnected(3 * 1000))
            {
                LOG_ERROR(QString("Cannot connect to server daemon @ %1:%2")
                    .arg(m_serverDaemonAddr.toString())
                    .arg(m_serverDaemonPort));
                return;
            }
            
            // Send request
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out << (qint32)task->getType();
            out << (quint16)task->getServerPort();

            socket.write(block);
            socket.waitForBytesWritten(-1);

            // 5
            // Receive response
            quint32 blockSize;
            QDataStream in(&socket);

            while( socket.bytesAvailable() < 4 )
            {
                if( !socket.waitForReadyRead(3 * 1000))
                {
                    LOG_WARN("Core application timed out");
                    return;
                }
            }
            in >> blockSize;
            while( socket.bytesAvailable() < blockSize )
            {
                if( !socket.waitForReadyRead(3 * 1000))
                {
                    LOG_WARN("Core application timed out");
                    return;
                }
            }
            
            bool ok;
            quint32 addr;
            QString description;

            in >> ok;
            
            // 6
            if( !ok )
            {
                in >> description;
                LOG_ERROR(description);
                return;
            }
            
            // 7
            in >> addr >> description;

            Client *client = ClientFactory::create(task->getType(), task, 
                                 addrTable[it.key()].toString(), 
                                 QHostAddress(addr).toString());
            if( client == NULL )
            {
                return;
            }
            else
            {
                clients.append(client);
            }
        }
    }
    
    
    // 8
    GlobalTimer::start();
    LOG_DEBUG("Starting clients...");
    
    for(int i = 0; i < clients.size(); i++)
    {
        clients[i]->start();
    }
    
    // 9
    LOG_DEBUG("Waiting till all client threads exit...");
    for(int i = 0; i < clients.size(); i++)
    {
        clients[i]->wait();
    }
    
    // 10
    if( TextTrace::enabled())
    {
        LOG_DEBUG("Generate trace file...");
        TextTrace::generate(tasks, clients); // This piece of code is odd
                                             // Should be improved later
    }
    
    LOG_DEBUG("End of CoreApplication::exec");
}

