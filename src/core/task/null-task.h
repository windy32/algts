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

#ifndef NULL_TASK_H
#define NULL_TASK_H

#include "task.h"

/**
 * \brief The null task
 *
 * The "Null Object Pattern" and the "Singleton Pettern" are used here in the
 * null task.
 *
 * It's used to simplify error handling in test scripts. For example, it's
 * common to add a task to the scenario, and then set its attribute.
 * \code
 * s.addTask("Harry", new BulkDownloadTask(80));
 * s.task()->setAttribute("MaxBytes", "1MB");
 * s.task()->setAttribute("MaxRate", "1Mbps");
 * \endcode
 * However, when there're not any tasks in the scenario, the call to s.task()
 * could have returned a NULL pointer, which would cause an access violation
 * and the process would be terminated immediately.
 *
 * The access violation could be avoided by checking for NULL pointers in the
 * script. Since we'd like to simplify the script as much as possible, however,
 * a better solution is returning a null task object.
 * \code
 * // s.addTask("Harry", new BulkDownloadTask(80));
 * s.task()->setAttribute("MaxBytes", "1MB"); // Now no access violation occurs, 
 * s.task()->setAttribute("MaxRate", "1Mbps"); // and a warning shows up instead
 * \endcode
 *
 * \see Scenario
 */
class NullTask : public Task
{
private:
    static NullTask *m_instance;
    NullTask();

public:
    /**
     * \brief Return the pointer to the single instance of NullTask
     * \return The pointer to the single instance of NullTask
     */
    static NullTask *getInstance();

public: // interface for script
    /**
     * \brief Generate a warning message and return immediately
     */
    virtual void setAttribute(const QString &attribute, const QString &value);

public:
    virtual enum Type getType();

    /**
     * \brief Generate a warning message and return immediately
     */
    virtual void serialize(QDataStream *stream);

    /**
     * \brief Generate a warning message and return immediately
     */
    virtual void expand(qint32 length);
};

#endif /* NULL_TASK_H */

