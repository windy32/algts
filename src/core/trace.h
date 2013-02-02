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

#ifndef TRACE_H
#define TRACE_H

#include "common.h"
#include "task/task.h"

/**
 * \file trace.h
 * \brief Global tracing support
 *
 * **Raw trace**
 *
 * Raw trace is generated when a client is executing a task, they are basically
 * simple event descriptions, e.g.:
 * "at 3692ms, 1430 bytes are received, now 7220024 bytes are received in total"
 *
 * \note Raw trace is generated internally when a client executes a task, 
 *       however it can be exported via exportRawTrace() with a core application
 *       or a console application object.
 *
 * **Text trace**
 *
 * Text trace (the trace file) is generated after the execution of tasks.
 * \code
 * username="Harry" type="On/Off Download"
 * {
 *   0 0 30 1000 1000
 *   0 1 99 1450 2450
 *   0 2 127 1450 3900
 *   0 3 183 1000 4900
 *   0 4 227 1000 5900
 *   0 5 472 1000 6900
 *   0 71 5532 1000 9900
 *   1 0 16882 1000 1000
 *   ...
 * }
 * 
 * username="Sally" type="Tcp Echo"
 * {
 *   ...
 * }
 * \endcode
 * 
 * **Regular trace**
 * 
 * The regular trace for a task may consists of several vectors, in each
 * vector, trace data are organized according to a specific time interval.
 * Currently, the time interval is one second.
 *
 * e.g., for a tcp echo task, there're 2 vectors, "Delay" and "Active".
 * In the "Delay" vector, the 1st element represents the average delay for the
 * 1st second, and the 2nd element represents the average delay for the 2nd
 * second, etc.
 * 
 * \see Client
 */

/**
 * \brief Raw trace output for a task
 * \ingroup Constructs
 */
typedef QMap<QString, QList<qint32> > RawTraceItem;

/**
 * \brief Text trace output for a task
 * \ingroup Constructs
 */
typedef QStringList TextTraceItem;

/**
 * \brief Regular trace output for a task
 * \ingroup Constructs
 */
typedef QMap<QString, QList<qint32> > RegularTraceItem;

class Client;

/**
 * \brief The global text trace object
 * \ingroup Constructs
 */
class TextTrace
{
private:
    static bool m_enabled;
    static QString m_filename;
    
public:
    /**
     * \brief Enable text trace
     * \param filename Name of the text trace file
     */
    static void enable(const QString &filename);
    /**
     * \brief Returns if text trace is enabled
     * \return Returns true if text trace is enabled, and false otherwise
     */
    static bool enabled();
    /**
     * \brief Generate text trace
     * \param tasks The tasks in the scenario
     * \param clients The clients started executing the tasks
     */
    static void generate(QMap<QString, QVector<Task *> > &tasks,
                         QList<Client *> &clients);
};

#endif /* TRACE_H */

