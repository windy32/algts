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

#ifndef TCP_ECHO_TASK_H
#define TCP_ECHO_TASK_H

#include "task.h"
#include "../random-variable-factory.h"

/**
 * \brief The tcp echo task
 * \ingroup Tasks
 */
class TcpEchoTask : public Task
{
private:
    RandomVariable *m_inputSize;
    RandomVariable *m_echoSize;
    RandomVariable *m_interval;
	
    QVector<qint32> m_inputSizes;
    QVector<qint32> m_echoSizes;
    QVector<qint32> m_intervals;

public:
    /**
     * \brief Initialize the tcp echo task
     * \param serverPort Server port of the task
     * \param startTime Start time of the task
     * \param stopTime Stop time of the task
     */
    TcpEchoTask(quint16 serverPort, qint32 startTime = 0, qint32 stopTime = -1);

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public: // interface for client
    /**
     * \brief Return the size of each input packets
     * \return The size (bytes) of each input packets
     */
    const QVector<qint32> &getInputSizes();

    /**
     * \brief Return the size of each echo packets
     * \return The size (bytes) of each echo packets
     */
    const QVector<qint32> &getEchoSizes();

    /**
     * \brief Return the length of each intervals
     * \return The length (ms) of each intervals
     */
    const QVector<qint32> &getIntervals();

public:
    friend QDataStream &operator<<(QDataStream &out, const TcpEchoTask &task);
    friend QDataStream &operator>>(QDataStream &in, TcpEchoTask &task);

public:
    virtual enum Type getType();
    virtual QString getName();
    virtual void expand();
};

QDataStream &operator<<(QDataStream &out, const TcpEchoTask &task);
QDataStream &operator>>(QDataStream &in, TcpEchoTask &task);

#endif /* TCP_ECHO_TASK_H */

