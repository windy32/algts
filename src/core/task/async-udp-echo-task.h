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

#ifndef ASYNC_UDP_ECHO_TASK_H
#define ASYNC_UDP_ECHO_TASK_H

#include "task.h"
#include "../random-variable-factory.h"

class AsyncUdpEchoTask : public Task
{
private:
    RandomVariable *m_inputSize;
    RandomVariable *m_echoSize;
    RandomVariable *m_interval;

    QVector<qint32> m_inputSizes;
    QVector<qint32> m_echoSizes;
    QVector<qint32> m_intervals;

public:
    AsyncUdpEchoTask(quint16 serverPort, qint32 startTime = 0, qint32 stopTime = -1);

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public: // interface for client
    const QVector<qint32> &getInputSizes();
    const QVector<qint32> &getEchoSizes();
    const QVector<qint32> &getIntervals();

public:
    virtual enum Type getType();
    virtual void serialize(QDataStream *pStream);
    virtual void expand(qint32 length);
};

#endif /* ASYNC_UDP_ECHO_TASK_H */
