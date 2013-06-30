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

#include "task.h"

#include "bulk-download-task.h"
#include "bulk-upload-task.h"
#include "onoff-download-task.h"
#include "tcp-echo-task.h"
#include "async-udp-echo-task.h"

Task::Task(quint16 serverPort, qint32 startTime, qint32 stopTime)
    : m_serverPort(serverPort), 
      m_startTime(startTime), 
      m_stopTime(stopTime)
{
}

quint16 Task::getServerPort()
{
    return m_serverPort;
}

qint32 Task::getStartTime()
{
    return m_startTime;
}

qint32 Task::getStopTime()
{
    return m_stopTime;
}

void Task::setStopTime(qint32 stopTime)
{
    m_stopTime = stopTime;
}

QDataStream &operator<<(QDataStream &out, Task* task)
{
    if( task->getType() == Task::BULK_DOWNLOAD )
    {
        out << (qint32)Task::BULK_DOWNLOAD;
        out << *(BulkDownloadTask *)task;
    }
    else if( task->getType() == Task::BULK_UPLOAD )
    {
        out << (qint32)Task::BULK_UPLOAD;
        out << *(BulkUploadTask *)task;
    }
    else if( task->getType() == Task::ONOFF_DOWNLOAD )
    {
        out << (qint32)Task::ONOFF_DOWNLOAD;
        out << *(OnoffDownloadTask *)task;
    }
    else if( task->getType() == Task::TCP_ECHO )
    {
        out << (qint32)Task::TCP_ECHO;
        out << *(TcpEchoTask *)task;
    }
    else if( task->getType() == Task::ASYNC_UDP_ECHO )
    {
        out << (qint32)Task::ASYNC_UDP_ECHO;
        out << *(AsyncUdpEchoTask *)task;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, Task* &task)
{
    qint32 type;
    in >> type;

    if( type == Task::BULK_DOWNLOAD )
    {
        task = (Task *)new BulkDownloadTask(0);
        in >> *(BulkDownloadTask *)task;
    }
    else if( type == Task::BULK_UPLOAD )
    {
        task = (Task *)new BulkUploadTask(0);
        in >> *(BulkUploadTask *)task;
    }
    else if( type == Task::ONOFF_DOWNLOAD )
    {
        task = (Task *)new OnoffDownloadTask(0);
        in >> *(OnoffDownloadTask *)task;
    }
    else if( type == Task::TCP_ECHO )
    {
        task = (Task *)new TcpEchoTask(0);
        in >> *(TcpEchoTask *)task;
    }
    else if( type == Task::ASYNC_UDP_ECHO )
    {
        task = (Task *)new AsyncUdpEchoTask(0);
        in >> *(AsyncUdpEchoTask *)task;
    }
    return in;
}

