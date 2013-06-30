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

#include "QTcpServerEx.h"
#include "log.h"

void QTcpServerEx::incomingConnection(int socketDescriptor)
{
    // Insert the socket descriptor
    m_mutex.lock();
    m_pendingDescriptors.enqueue(socketDescriptor);
    m_mutex.unlock();
}

int QTcpServerEx::nextPendingDescriptor()
{
    int result = 0;
    
    m_mutex.lock();
    if( m_pendingDescriptors.isEmpty())
    {
        LOG_ERROR("QTcpServerEx: No pending socket descriptor available");
    }
    else
    {
        result = m_pendingDescriptors.dequeue();
    }
    m_mutex.unlock();
    
    return result;
}

bool QTcpServerEx::hasPendingDescriptor()
{
    return !m_pendingDescriptors.isEmpty();
}

