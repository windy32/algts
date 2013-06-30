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

#ifndef SCENARIO_H
#define SCENARIO_H

#include "common.h"
#include "task/task.h"

/**
 * \ingroup Constructs
 * \brief The scenario
 * 
 * A scenario describes how a bottle-neck link is shared between different
 * agencies, protocol families, or applications, etc.
 *
 * Consider the scenario below:
 *
 * User  | Application            | Time 
 * ------|------------------------|--------------
 * Harry | eMule                  | 00:00 ~ 24:00
 * Harry | Chromium               | 20:00 ~ 21:00
 * Sally | Skype video conference | 14:00 ~ 15:00
 *
 * Harry and Sally share a DSL link, with a downstream rate at 2Mbps, and an 
 * upstream rate at 256Kbps.
 *
 * Harry is running his eMule all the time, and starts his chromium browser at
 * 20:00 checking for emails, rss seeds, etc. While Sally just has a video
 * conference in the afternoon.
 * 
 * To test the QoS policy enforced at the gateway, we could set up a relatively
 * shorter and more abstract scenario.
 * 
 * \code
 * Scenario s(12345, 60 * 10); // Set seed to 12345, and length to 10 minutes
 * \endcode
 * 
 * And then add users and tasks to the scenario.
 * 
 * \code
 * s.addUser("Harry");
 * s.addTask("Harry", ...);
 * ...
 * s.addUser("Sally");
 * s.addTask("Sally, ...);
 * ...
 * \endcode
 *
 * A task is somewhat similar to a network application. It's convenient to
 * set the start time, stop time to a task, and assign different attributes to
 * tasks so that the scenario object can better resemble the real scenario.
 * 
 * Finally, the scenario can be executed in an application which means that
 * sockets will be created for different tasks, and packets will be sent to
 * network interfaces.
 * 
 * \see Task, CoreApplication
 */
class Scenario
{
protected:
    // Task related members
    qint32 m_seed;
    qint32 m_length;
    QMap<QString, QVector<Task *> > m_tasks;
    
    // Most recently inserted task
    Task *m_lastTask;

public: // Constructor
    /**
     * \brief Initialize the scenario
     * \param seed The seed of the scenario
     * \param length The length (second) of the scenario
     */
    Scenario(qint32 seed, qint32 length);

public: // Task management interfaces for scripting
    /**
     * \brief Add a user to the scenario
     * \param userName The name of the user
     */
    void addUser(const QString &userName);
    /**
     * \brief Add a task to the scenario
     * \param userName The user that the task belongs to
     * \param task Pointer to the task
     */
    void addTask(const QString &userName, Task *task);
    
    /**
     * \brief Returns the most recently inserted task
     * \return The most recently inserted task
     * \note If no task is available, a null-task is returned
     */
    Task *task();

public: // One extra for resetting stop time -1 to scenario length
    /**
     * \brief Get the length of the scenario
     * \return The length of the scenario in seconds
     */
    qint32 getLength();

public:
    friend QDataStream &operator<<(QDataStream &out, const Scenario &scenario);
    friend QDataStream &operator>>(QDataStream &in, Scenario &scenario);

public:
    /**
     * \brief Returns all tasks in the scenario
     * \return All tasks in the scenario
     */
    QMap<QString, QVector<Task *> > &tasks();
};

QDataStream &operator<<(QDataStream &out, const Scenario &scenario);
QDataStream &operator>>(QDataStream &in, Scenario &scenario);

#endif /* SCENARIO_H */

