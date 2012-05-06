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

#include "task.h"

Task::Task(quint16 serverPort, qint32 startTime, qint32 stopTime)
    : m_serverPort(serverPort), 
      m_startTime(startTime), 
      m_stopTime(stopTime)
{
}

void Task::serialize(QDataStream &stream)
{
    if( stream.device()->openMode() == QIODevice::ReadOnly )
    {
        stream >> m_serverPort >> m_startTime >> m_stopTime;
    }
    else if( stream.device()->openMode() == QIODevice::WriteOnly )
    {
        stream << m_serverPort << m_startTime << m_stopTime;
    }
}

quint16 Task::getServerPort()
{
    return m_serverPort;
}

qint32 Task::getStartTime()
{
    return m_startTime;
}

qint32 Task::getStopTime()
{
    return m_stopTime;
}

void Task::setStopTime(qint32 stopTime)
{
    m_stopTime = stopTime;
}

