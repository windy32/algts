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

void Scenario::serialize(QDataStream *pStream)
{
    LOG_DEBUG("Scenario::serialize() not implemented yet");
}
