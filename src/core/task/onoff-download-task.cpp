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

#include "onoff-download-task.h"

OnoffDownloadTask::OnoffDownloadTask(quint16 serverPort, qint32 startTime, 
    qint32 stopTime)
    : Task(serverPort, startTime, stopTime)
{
    m_maxRate = -1;
    LOG_DEBUG("MaxRate set to default value INFINITE");
    
    m_packetSize = 1200;
    LOG_DEBUG("PacketSize set to default value 1200");
    
    m_requestSize = 100;
    LOG_DEBUG("RequestSize set to default value 100");
    
    m_onTime = NullVariable::getInstance();
    m_offTime = NullVariable::getInstance();
}

void OnoffDownloadTask::setAttribute(const QString &attribute, 
                                     const QString &value)
{
    LOG_DEBUG("Beginning of OnoffDownloadTask::setAttribute");

    if( attribute == "MaxRate" )
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
    else if( attribute == "PacketSize" )
    {
        // Capture number and unit
        QString number, unit;
        QRegExp rx("^([0-9]+)(K|)B$"); // case sensitive by default

        if( rx.indexIn(value) == -1 )
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        number = rx.cap(1); // >= 8B and <= 32KB
        unit = rx.cap(2);
        
        // Convert to bytes with limitation
        bool ok;
        qint32 intValue = number.toLong(&ok);
        qint32 inc = ( unit == "K" ) ? 1024 : 1;

        if( !ok || // cannot convert to long
            intValue > 32 * 1024 / inc ) // or larger than 32KB
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        else
        {
            m_packetSize = intValue * inc;
        }
    }
    else if( attribute == "RequestSize" )
    {
        // Capture number and unit
        QString number, unit;
        QRegExp rx("^([0-9]+)(K|)B$"); // case sensitive by default

        if( rx.indexIn(value) == -1 )
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        number = rx.cap(1); // >= 8B and <= 1KB
        unit = rx.cap(2);
        
        // Convert to bytes with limitation
        bool ok;
        qint32 intValue = number.toLong(&ok);
        qint32 inc = ( unit == "K" ) ? 1024 : 1;

        if( !ok || // cannot convert to long
            intValue > 1 * 1024 / inc ) // or larger than 1KB
        {
            LOG_WARN(QString("Invalid value %1").arg(value));
            return;
        }
        else
        {
            m_requestSize = intValue * inc;
        }
    }
    else if( attribute == "OnTime" )
    {
        if( m_onTime != NULL && m_onTime != NullVariable::getInstance())
        {
            delete m_onTime;
        }
        m_onTime = RandomVariableFactory::create(value);
    }
    else if( attribute == "OffTime" )
    {
        if( m_offTime != NULL && m_offTime != NullVariable::getInstance())
        {
            delete m_offTime;
        }
        m_offTime = RandomVariableFactory::create(value);
    }
    else
    {
        LOG_WARN(QString("Attribute %s not supported").arg(attribute));
    }
    LOG_DEBUG("End of OnoffDownloadTask::setAttribute");
}

qint32 OnoffDownloadTask::getMaxRate()
{
    return m_maxRate;
}

qint16 OnoffDownloadTask::getPacketSize()
{
    return m_packetSize;
}

qint16 OnoffDownloadTask::getRequestSize()
{
    return m_requestSize;
}

const QVector<qint32> &OnoffDownloadTask::getOnTimes()
{
    return m_onTimes;
}

const QVector<qint32> &OnoffDownloadTask::getOffTimes()
{
    return m_offTimes;
}

void OnoffDownloadTask::setMaxRate(qint32 maxRate)
{
    m_maxRate = maxRate;
}

void OnoffDownloadTask::setPacketSize(qint16 packetSize)
{
    m_packetSize = packetSize;
}

void OnoffDownloadTask::setRequestSize(qint16 requestSize)
{
    m_requestSize = requestSize;
}

RandomVariable *OnoffDownloadTask::getOnTime()
{
    return m_onTime;
}

RandomVariable *OnoffDownloadTask::getOffTime()
{
    return m_offTime;
}

enum Task::Type OnoffDownloadTask::getType()
{
    return ONOFF_DOWNLOAD;
}

QString OnoffDownloadTask::getName()
{
    return "On/Off Download Task";
}

void OnoffDownloadTask::expand()
{
    m_offTimes.clear();
    m_onTimes.clear();

    int length = m_stopTime;
    int curLength = 0;
    while( curLength < length )
    {
        qint32 offTime = m_offTime->getInteger();
        qint32 onTime = m_onTime->getInteger();
        
        offTime = qMin(offTime, length);
        onTime = qMin(onTime, length);
        
        m_offTimes.push_back(offTime);
        m_onTimes.push_back(onTime);
        
        curLength += offTime + onTime; // It may overflow in some cases
    }
}

QDataStream &operator<<(QDataStream &out, const OnoffDownloadTask &task)
{
    out << task.m_serverPort << task.m_startTime << task.m_stopTime;
    out << task.m_maxRate << task.m_packetSize << task.m_requestSize;
    out << task.m_onTime << task.m_offTime;
    return out;
}

QDataStream &operator>>(QDataStream &in, OnoffDownloadTask &task)
{
    QString onTime, offTime;
    in >> task.m_serverPort >> task.m_startTime >> task.m_stopTime;
    in >> task.m_maxRate >> task.m_packetSize >> task.m_requestSize;
    in >> onTime >> offTime;
    
    task.m_onTime = RandomVariableFactory::create(onTime);
    task.m_offTime = RandomVariableFactory::create(offTime);
    return in;
}

