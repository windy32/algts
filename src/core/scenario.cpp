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

#include "scenario.h"
#include "task/null-task.h"

Scenario::Scenario(qint32 seed, qint32 length)
    : m_seed(seed), 
      m_length(length),
      m_lastTask(NULL)
{
}

void Scenario::addUser(const QString &userName)
{
    if( m_tasks.contains(userName))
    {
        LOG_WARN(QString("User %1 exists").arg(userName));
    }
    else
    {
        m_tasks.insert(userName, QVector<Task *>());
    }
}

void Scenario::addTask(const QString &userName, Task *task)
{
    if( m_tasks.contains(userName))
    {
        m_tasks[userName].push_back(task);
        m_lastTask = task;
        
        if( task->getStopTime() == -1 )
        {
            task->setStopTime(m_length * 1000);
        }
    }
    else
    {
        LOG_WARN(QString("User %1 does not exist").arg(userName));
    }
}

Task *Scenario::task()
{
    if( m_lastTask )
    {
        return m_lastTask;
    }
    else
    {
        LOG_WARN("No task available");
        return NullTask::getInstance();
    }
}

QMap<QString, QVector<Task *> > &Scenario::tasks()
{
    return m_tasks;
}

void Scenario::serialize(QDataStream *stream)
{
    // Format
    // int32 seed
    // int32 length
    // tasks | - int32       number-of-tasks
    //       |------------------------------
    //       | - QString     username1
    //       | - int32       task1-1-type
    //       | - Task-Object task1-1
    //       | - QString     username1
    //       | - Task-Object task1-2-type
    //       | - Task-Object task1-2
    //       | - ...
    //       | - QString     username2
    //       | - int32       task2-1-type
    //       | - Task-Object task2-1
    //       | - ...
    int32 count = 0;
    
    if( stream->device()->openMode == QIODevice::ReadOnly )
    {
        stream >> m_seed >> m_length;
        stream >> count;
        
        for(int i = 0; i < count; i++)
        {
            QString username;
            int32 type;
            Task *task;
            
            stream >> username >> type;
            if( type == (int32)Task::BULK_DOWNLOAD )
            {
                task = new BulkDownloadTask(0);
            }
            else if( type == (int32)Task::BULK_UPLOAD )
            {
                task = new BulkUploadTask(0);
            }
            else if( type == (int32)Task::ONOFF_DOWNLOAD )
            {
                task = new OnoffDownloadTask(0);
            }
            else if( type == (int32)Task::TCP_ECHO )
            {
                task = new TcpEchoTask(0);
            }
            else if( type == (int32)Task::ASYNC_UDP_ECHO )
            {
                task = new UdpEchoTask(0);
            }
            task->serialize(stream);
            
            //if( !m_tasks.contains(username))
            //{
            //    m_tasks.insert(username, QVector<Task *>());
            //}
            m_tasks[username].append(task);
        }
    }
    else if( stream->device()->openMode == QIODevice::WriteOnly )
    {
        stream << m_seed << m_length;
        
        QMap<QString, QVector<Task *> >::iterator it;
        for(it = tasks.begin(); it != tasks.end(); ++it)
        {
            for(int i = 0; i < it.value().size(); i++)
            {
                count += 1;
            }
        }

        stream << count;
        for(it = tasks.begin(); it != tasks.end(); ++it)
        {
            for(int i = 0; i < it.value().size(); i++)
            {
                Task *task = it.value()[i];
                stream << it.key();
                stream << (qint32)task->getType();
                task->serialize(stream);
            }
        }
    }
}
