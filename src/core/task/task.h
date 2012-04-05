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

#ifndef TASK_H
#define TASK_H

#include "../common.h"

class Task : public Serializable
{
protected:
    quint16 m_serverPort;
    qint32 m_startTime;
    qint32 m_stopTime;

public:
    enum Type
    {
        BULK_DOWNLOAD  = 0,
        BULK_UPLOAD    = 1,
        ONOFF_DOWNLOAD = 2,
        TCP_ECHO       = 3,
        ASYNC_UDP_ECHO = 4,
        OTHER
    };

public:
    Task(quint16 serverPort, qint32 startTime = 0, qint32 stopTime = -1);
    
    // Set task-specific attributes
    virtual void setAttribute(const QString &attribute, 
                              const QString &value) = 0;
    
    // Serialization of tasks should alwasys occur before expanding, 
    // no expanded values are serialized
    virtual void serialize(QDataStream *pStream) = 0;

	// Expand random variables to concrete values
    virtual void expand(qint32 length) = 0;
	
    virtual enum Type getType() = 0;
    
    quint16 getServerPort();
    qint32  getStartTime();
    qint32  getStopTime();
};

#endif /* TASK_H */
