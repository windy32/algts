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

#include "bulk-upload-task.h"

BulkUploadTask::BulkUploadTask(quint16 serverPort, qint32 startTime, 
    qint32 stopTime)
    : Task(serverPort, startTime, stopTime)
{
    m_maxBytes = -1;
    LOG_DEBUG("MaxBytes set to default value INFINITE");
    
    m_maxRate = -1;
    LOG_DEBUG("MaxRate set to default value INFINITE");
}

void BulkUploadTask::setAttribute(const QString &attribute, 
                                  const QString &value)
{
    LOG_DEBUG("Beginning of BulkUploadTask::setAttribute");

    if( attribute == "MaxBytes" )
    {
        if( value == "INFINITE" )
        {
            m_maxBytes = -1;
            return;
        }
        
        // Capture number and unit
        QString number, unit;
        QRegExp rx("^([0-9]+)(G|M|K|)B$"); // case sensitive by default

        if( rx.indexIn(value) == -1 )
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        number = rx.cap(1); // > 0B and <= 2GB
        unit = rx.cap(2);
        
        // Convert to bytes with limitation
        bool ok;
        qint32 intValue = number.toLong(&ok);
        qint32 inc = 
            ( unit == "G" ) ? 1024 * 1024 * 1024 : 
            ( unit == "M" ) ? 1024 * 1024 :
            ( unit == "K" ) ? 1024 : 1;

        if( !ok || // cannot convert to long, 
            intValue == 0 || // no bytes to send, or larger than 2GB
            intValue > ((qint64)2 * 1024 * 1024 * 1024) / inc )
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        else
        {
            m_maxBytes = intValue * inc;
        }
    }
    else if( attribute == "MaxRate" )
    {
        if( value == "INFINITE" )
        {
            m_maxRate = -1;
            return;
        }
        
        // Capture number and unit
        QString number, unit;
        QRegExp rx("^([0-9]+)(m|M|k|K|)bps$");

        if( rx.indexIn(value) == -1 )
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        number = rx.cap(1); // > 0B and <= 2GB
        unit = rx.cap(2);

        // Convert to byte/s with limitation
        bool ok;
        qint32 intValue = number.toLong(&ok);
        qint32 inc = 
            ( unit == "M" || unit == "m" ) ? 1024 * 1024 :
            ( unit == "K" || unit == "k" ) ? 1024 : 1;

        if( !ok || // cannot convert to long
            intValue > 100 * 1024 * 1024 / inc ) // or larger than 100Mbps
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        else
        {
            m_maxRate = intValue * inc / 8;
        }
    }
    else
    {
        LOG_WARN(QString("Attribute %s not supported").arg(attribute));
    }
    LOG_DEBUG("End of BulkUploadTask::setAttribute");
}

qint32 BulkUploadTask::getMaxBytes()
{
    return m_maxBytes;
}

qint32 BulkUploadTask::getMaxRate()
{
    return m_maxRate;
}

void BulkUploadTask::setMaxBytes(qint32 maxBytes)
{
    m_maxBytes = maxBytes;
}

void BulkUploadTask::setMaxRate(qint32 maxRate)
{
    m_maxRate = maxRate;
}

enum Task::Type BulkUploadTask::getType()
{
    return BULK_UPLOAD;
}

QString BulkUploadTask::getName()
{
    return "Bulk Upload Task";
}

void BulkUploadTask::expand()
{
    // nothing to do here
}

QDataStream &operator<<(QDataStream &out, const BulkUploadTask &task)
{
    out << task.m_serverPort << task.m_startTime << task.m_stopTime;
    out << task.m_maxBytes << task.m_maxRate;
    return out;
}

QDataStream &operator>>(QDataStream &in, BulkUploadTask &task)
{
    in >> task.m_serverPort >> task.m_startTime >> task.m_stopTime;
    in >> task.m_maxBytes >> task.m_maxRate;
    return in;
}

