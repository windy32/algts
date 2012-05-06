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

#ifndef BULK_UPLOAD_TASK_H
#define BULK_UPLOAD_TASK_H

#include "task.h"

/**
 * \brief The bulk upload task
 * \ingroup Tasks
 */
class BulkUploadTask : public Task
{
private:
    qint32 m_maxBytes;
    qint32 m_maxRate;

public:
    /**
     * \brief Initialize the bulk upload task
     * \param serverPort Server port of the task
     * \param startTime Start time of the task
     * \param stopTime Stop time of the task
     */
    BulkUploadTask(quint16 serverPort, qint32 startTime = 0, 
        qint32 stopTime = -1);

public: // interface for script
    virtual void setAttribute(const QString &attribute, const QString &value);

public: //interface for client
    /**
     * \brief Return max bytes to upload
     * \return Max bytes to upload
     */
    qint32 getMaxBytes();

    /**
     * \brief Return max upload rate
     * \return Max upload rate (byte/s)
     */
    qint32 getMaxRate();

public:
    friend QDataStream &operator<<(QDataStream &out, const BulkUploadTask &task);
    friend QDataStream &operator>>(QDataStream &in, BulkUploadTask &task);

public:
    virtual enum Type getType();
    virtual QString getName();
    virtual void expand();
};

QDataStream &operator<<(QDataStream &out, const BulkUploadTask &task);
QDataStream &operator>>(QDataStream &in, BulkUploadTask &task);

#endif /* BULK_UPLOAD_TASK_H */

