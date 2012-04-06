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

#ifndef SCENARIO_H
#define SCENARIO_H

#include "common.h"
#include "task/task.h"

class Scenario : public Serializable
{
private:
    // Task related members
    qint32 m_seed;
    qint32 m_length;
    QMap<QString, QVector<Task *> > m_tasks;
    
    // Most recently inserted task
    Task *m_lastTask;

public: // Constructor
    Scenario(qint32 seed, qint32 length);

public: // Task management interfaces for scripting
    void addUser(const QString &userName);
    void addTask(const QString &userName, Task *task);
    
    // Return the most recently inserted task
    // if no task is available, a null-task is returned
    Task *task();

public:
    QMap<QString, QVector<Task *> > &tasks();

public:
    virtual void serialize(QDataStream *stream);
};

#endif /* SCENARIO_H */
