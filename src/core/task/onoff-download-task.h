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
    
public: // interface for gui client
    /**
     * \brief Set max download rate
     * \param maxRate Max download rate (byte/s)
     */
    void setMaxRate(qint32 maxRate);
    /**
     * \brief Set application layer packet size
     * \param packetSize Application layer packet size in bytes
     */
    void setPacketSize(qint16 packetSize);
    /**
     * \brief Set request packet size
     * \param requestSize Request size in bytes
     */
    void setRequestSize(qint16 requestSize);

    /**
     * \brief Return the random variable OnTime
     * \return Pointer to the random variable
     */
    RandomVariable *getOnTime();
    /**
     * \brief Return the random variable OffTime
     * \return Pointer to the random variable
     */
    RandomVariable *getOffTime();

public:
    friend QDataStream &operator<<(QDataStream &out, const OnoffDownloadTask &task);
    friend QDataStream &operator>>(QDataStream &in, OnoffDownloadTask &task);

public:
    virtual enum Type getType();
    virtual QString getName();
    virtual void expand();
};

QDataStream &operator<<(QDataStream &out, const OnoffDownloadTask &task);
QDataStream &operator>>(QDataStream &in, OnoffDownloadTask &task);

#endif /* ONOFF_DOWNLOAD_TASK_H */

