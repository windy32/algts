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

#ifndef TASK_H
#define TASK_H

#include "../common.h"

/**
 * \defgroup Tasks Tasks
 * \ingroup Core
 */

/**
 * \brief The base class of all tasks
 * \ingroup Tasks
 */
class Task : public Serializable
{
protected:
    /**
     * \brief Port of the server
     */
    quint16 m_serverPort;
    /**
     * \brief Start time of the task
     */
    qint32 m_startTime;
    /**
     * \brief Stop time of the task
     */
    qint32 m_stopTime;

public:
    /**
     * \brief The type of a task.
     */
    enum Type
    {
        BULK_DOWNLOAD  = 0,
        BULK_UPLOAD    = 1,
        ONOFF_DOWNLOAD = 2,
        TCP_ECHO       = 3,
        ASYNC_UDP_ECHO = 4,
        OTHER
    };

public:
    /**
     * \brief Initialize the base class with specified server port, start time
     *        and stop time.
     * \param serverPort Server port of the task
     * \param startTime Start time of the task
     * \param stopTime Stop time of the task
     */
    Task(quint16 serverPort, qint32 startTime = 0, qint32 stopTime = -1);
    
    /**
     * \brief Set the task attribute
     * \param attribute Name of the attribute
     * \param value Value of the attribute
     */
    virtual void setAttribute(const QString &attribute, 
                              const QString &value) = 0;
    
    /**
     * \brief Serialize the task object
     * \param stream The target device of the serialization
     */
    virtual void serialize(QDataStream *stream) = 0;

    /**
     * \brief Expand the task object
     * \param length Max length of the tasks (ms)
     *
     * Some tasks have random number generator members, expanding means 
     * generating a random number sequence.
     *
     * \note The order creating task objects as well as calling expand functions
     *       will both lead to different expanding results.
     */
    virtual void expand(qint32 length) = 0;
	
    /**
     * \brief Return the type of the task 
     * \return Type of the task
     */
    virtual enum Type getType() = 0;
    
    /**
     * \brief Return the server port of the task 
     * \return Server port of the task
     */
    quint16 getServerPort();

    /**
     * \brief Return the start time of the task 
     * \return Start time (ms) of the task
     */
    qint32  getStartTime();
    
    /**
     * \brief Return the stop time of the task 
     * \return Stop time (ms) of the task
     */
    qint32  getStopTime();
};

#endif /* TASK_H */

