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

#include "async-udp-echo-task.h"

AsyncUdpEchoTask::AsyncUdpEchoTask(quint16 serverPort, qint32 startTime, 
    qint32 stopTime)
    : Task(serverPort, startTime, stopTime)
{
    m_inputSize = NullVariable::getInstance();
    m_echoSize = NullVariable::getInstance();
    m_interval = NullVariable::getInstance();
}

void AsyncUdpEchoTask::setAttribute(const QString &attribute, 
                                    const QString &value)
{
    LOG_DEBUG("Beginning of AsyncUdpEchoTask::setAttribute");

    if( attribute == "InputSize" )
    {
        m_inputSize = RandomVariableFactory::create(value);
        if( m_inputSize->getMin() < 6 )
        {
            LOG_WARN("Minimum value of InputSize is less than 6B");
        }
        else if( m_inputSize->getMax() > 1024 )
        {
            LOG_WARN("Maximum value of InputSize is greater than 1KB"); 
        }
    }
    else if( attribute == "EchoSize" )
    {
        m_echoSize = RandomVariableFactory::create(value);
        if( m_echoSize->getMin() < 4 )
        {
            LOG_WARN("Minimum value of EchoSize is less than 4B");
        }
        else if( m_echoSize->getMax() > 1472 )
        {
            LOG_WARN("Maximum value of EchoSize is greater than 1472B"); 
        }
    }
    else if( attribute == "Interval" )
    {
        m_interval = RandomVariableFactory::create(value);
    }
    else
    {
        LOG_WARN(QString("Attribute %s not supported").arg(attribute));
    }
    LOG_DEBUG("End of AsyncUdpEchoTask::setAttribute");
}

const QVector<qint32> &AsyncUdpEchoTask::getInputSizes()
{
    return m_inputSizes;
}

const QVector<qint32> &AsyncUdpEchoTask::getEchoSizes()
{
    return m_echoSizes;
}

const QVector<qint32> &AsyncUdpEchoTask::getIntervals()
{
    return m_intervals;
}

enum Task::Type AsyncUdpEchoTask::getType()
{
    return ASYNC_UDP_ECHO;
}

QString AsyncUdpEchoTask::getName()
{
    return "Asynchronous UDP Echo Task";
}

void AsyncUdpEchoTask::serialize(QDataStream *stream)
{
    LOG_DEBUG("AsyncUdpEchoTask::serialize() not implemented yet");
}

void AsyncUdpEchoTask::expand()
{
    int length = m_stopTime;
    int curLength = 0;
    while( curLength < length )
    {
        qint32 inputSize = m_inputSize->getInteger();
        qint32 echoSize = m_echoSize->getInteger();
        qint32 interval = m_interval->getInteger();
        
        inputSize = qMax(inputSize, 4);
        inputSize = qMin(inputSize, 1024);
        echoSize = qMax(echoSize, 1);
        echoSize = qMax(echoSize, 1472);
        interval = qMin(interval, length);
        
        m_inputSizes.push_back(inputSize);
        m_echoSizes.push_back(echoSize);
        m_intervals.push_back(interval);
        
        curLength += interval; // It may overflow in some cases
    }
}
