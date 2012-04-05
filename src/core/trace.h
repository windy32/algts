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

// Raw trace items are generated when a client is executing a task,
// those items are basically simple events, e.g.,
// "at 3692ms, 1430 bytes are received, now 7220024 bytes are received in total"

// Text trace items (trace files) are generated after the execution of tasks

/**
 * Format of trace files
 * 
 * username="Harry" type="onoff-download"
 * {
 *   0, 0, 30, 1000, 1000
 *   0, 1, 99, 1450, 2450
 *   0, 2, 127, 1450, 3900
 *   0, 3, 183, 1000, 4900
 *   0, 4, 227, 1000, 5900
 *   0, 5, 472, 1000, 6900
 *   0, 71, 5532, 1000, 9900
 *   1, 0, 16882, 1000, 1000
 *   ...
 * }
 * 
 * username="Sally" type="tcp-echo"
 * {
 *   ...
 * }
 *
 */
typedef QStringList TextTraceItem;

// Regular trace items are generated after the execution of tasks
// used for rating gui
typedef QMap<QString, QList<qint32> > RegularTraceItem;

class TextTrace
{
private:
    static bool m_enabled;
    static QString m_filename;
    
public:
    static void enable(const QString &filename);
    static bool enabled();
    static void generate(QMap<QString, QVector<Task *> > &tasks);
};

#endif /* TRACE_H */
