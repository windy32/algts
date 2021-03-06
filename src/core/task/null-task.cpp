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

#include "null-task.h"

NullTask *NullTask::m_instance = NULL;

NullTask::NullTask()
    : Task(0, 0, -1)
{
}

NullTask *NullTask::getInstance()
{
    if( m_instance == NULL )
    {
        m_instance = new NullTask();
    }
    return m_instance;
}

void NullTask::setAttribute(const QString & /*attribute*/, const QString &/*value*/)
{
    LOG_WARN("Call to NullTask::setAttribute()");
}

enum Task::Type NullTask::getType()
{
    return OTHER;
}

QString NullTask::getName()
{
    return "Null Task";
}

void NullTask::expand()
{
    LOG_WARN("Call to NullTask::expand()");
}
