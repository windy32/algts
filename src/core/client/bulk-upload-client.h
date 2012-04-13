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

#ifndef BULK_UPLOAD_CLIENT_H
#define BULK_UPLOAD_CLIENT_H

#include "client.h"
#include "../task/bulk-upload-task.h"

/**
 * \brief The bulk upload task client
 * \ingroup Clients
 *
 * Regular trace format:
 *  - vector<int32> TxRate (byte/s)
 *  - vector<int32> MaxTxRate (byte/s)
 *  - vector<int32> Active (BOOL)
 *
 * Text trace format:
 *  - index
 *  - time (ms)
 *  - newBytes
 *  - totalBytes
 *
 * An example of text trace output:
 * \code
 * 0 18 0 0                <- connecting to server (first line)
 * 1 40 1000 1000          <- first data packet sent
 * 2 99 1450 2450
 * 3 127 1450 3900
 * 4 183 1000 4900
 * 5 227 1000 5900
 * 6 472 1000 6900
 * ...
 * 183 134390 1000 3249320 <- last packet sent (last line)
 * \endcode
 */
class BulkUploadClient : public Client
{
protected:
    /**
     * \brief Raw trace structure for bulk upload tasks
     */
    struct RawTrace
    {
        /** \brief Index of the event that starts with 0 */
        QList<qint32> index;
        
        /** \brief The time (ms) of the event */
        QList<qint32> time;
        
        /** \brief Bytes sent in the event */
        QList<qint32> newBytes;
        
        /** \brief Total bytes sent */
        QList<qint32> totalBytes;
    };
    
    /**
     * \brief The raw trace object
     */
    RawTrace m_trace;
    
    /**
     * \brief Pointer to the bulk download task object
     */
    BulkUploadTask *m_task;

private:
	virtual void run();

public:
    /**
     * \brief Initialize the bulk upload client
     * \param localAddr Client's IPv4 address
     * \param serverAddr Server's IPv4 address
     * \param task Pointer to the input task object
     */
    BulkUploadClient(const QString &localAddr, 
                     const QString &serverAddr, BulkUploadTask *task);
    
    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

#endif /* BULK_UPLOAD_CLIENT_H */
