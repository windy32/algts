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

#ifndef ONOFF_DOWNLOAD_TASK_H
#define ONOFF_DOWNLOAD_TASK_H

#include "task.h"
#include "../random-variable-factory.h"

/**
 * \brief The on/off download task
 * \ingroup Tasks
 */
class OnoffDownloadTask : public Task
{
private:
    RandomVariable *m_onTime;
    RandomVariable *m_offTime;
    qint32 m_maxRate;
    qint16 m_packetSize;
    qint16 m_requestSize;

    QVector<qint32> m_onTimes;
    QVector<qint32> m_offTimes;

public:
    /**
     * \brief Initialize the on/off download task
     * \param serverPort Server port of the task
     * \param startTime Start time of the task
     * \param stopTime Stop time of the task
     */
    OnoffDownloadTask(quint16 serverPort, qint32 startTime = 0, 
        qint32 stopTime = -1);

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public: // interface for client
    /**
     * \brief Return max bytes to download
     * \return Max bytes to download
     */
    qint32 getMaxRate();

    /**
     * \brief Return the application layer packet size
     * \return The application layer packet size (bytes)
     */
    qint16 getPacketSize();

    /**
     * \brief Return the request size
     * \return The request size (bytes)
     */
    qint16 getRequestSize();

    /**
     * \brief Return the length of each "On" periods
     * \return The length (ms) of each "On" periods
     */
    const QVector<qint32> &getOnTimes();

    /**
     * \brief Return the length of each "Off" periods
     * \return The length (ms) of each "Off" periods
     */
    const QVector<qint32> &getOffTimes();

public:
    virtual enum Type getType();
    virtual void serialize(QDataStream *stream);
    virtual void expand(qint32 length);
};

#endif /* ONOFF_DOWNLOAD_TASK_H */

