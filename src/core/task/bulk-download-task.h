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

#ifndef BULK_DOWNLOAD_TASK_H
#define BULK_DOWNLOAD_TASK_H

#include "task.h"

/**
 * \brief The bulk download task
 * \ingroup Tasks
 */
class BulkDownloadTask : public Task
{
private:
    qint32 m_maxBytes;
    qint32 m_maxRate;

public:
    /**
     * \brief Initialize the bulk download task
     * \param serverPort Server port of the task
     * \param startTime Start time of the task
     * \param stopTime Stop time of the task
     */
    BulkDownloadTask(quint16 serverPort, qint32 startTime = 0, 
        qint32 stopTime = -1);

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public: //interface for client
    /**
     * \brief Return max bytes to download
     * \return Max bytes to download
     */
    qint32 getMaxBytes();

    /**
     * \brief Return max download rate
     * \return Max download rate (byte/s)
     */
    qint32 getMaxRate();

public:
    friend QDataStream &operator<<(QDataStream &out, const BulkDownloadTask &task);
    friend QDataStream &operator>>(QDataStream &in, BulkDownloadTask &task);

public:
    virtual enum Type getType();
    virtual QString getName();
    virtual void expand();
};

QDataStream &operator<<(QDataStream &out, const BulkDownloadTask &task);
QDataStream &operator>>(QDataStream &in, BulkDownloadTask &task);

#endif /* BULK_DOWNLOAD_TASK_H */

