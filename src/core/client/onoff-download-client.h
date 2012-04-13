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

#ifndef ONOFF_DOWNLOAD_CLIENT_H
#define ONOFF_DOWNLOAD_CLIENT_H

#include "client.h"
#include "../task/onoff-download-task.h"

/**
 * \brief The on/off download task client
 * \ingroup Clients
 *
 * Regular trace format:
 *  - vector<int32> RxRate (byte/s)
 *  - vector<int32> MaxRxRate (byte/s)
 *  - vector<int32> Active (BOOL)
 *
 * Text trace format:
 *  - period
 *  - index
 *  - time (ms)
 *  - newBytes
 *  - totalBytes
 *
 * An example of text trace output:
 * \code
 * 0, 0, 11, 0, 0           <- connecting to server
 * 0, 1, 30, 1000, 1000     <- first data packet receive in the "On" period
 * 0, 2, 99, 1450, 2450
 * 0, 3, 127, 1450, 3900
 * 0, 4, 183, 1000, 4900
 * 0, 5, 227, 1000, 5900
 * 0, 6, 472, 1000, 6900
 * ...
 * 0, 71, 5532, 1000, 59900 <- last data packet received in the "On" period
 *                          <- sleep during the "Off" period
 *
 * 1, 0, 16882, 0, 0 // connecting to server 
 *                     (first line in the next "On" period)
 * ...
 * \endcode
 */
class OnoffDownloadClient : public Client
{
protected:
    /**
     * \brief Raw trace structure for on/off download tasks
     */
    struct RawTrace
    {
        /** \brief Index of the "On" period */
        QVector<qint32> period;
        
        /** \brief Index of the event in current "On" period 
         *         that starts with 0
         */
        QVector<qint32> index;

        /** \brief The time (ms) of the event */
        QVector<qint32> time;

        /** \brief Bytes received in the event */
        QVector<qint32> newBytes;

        /** \brief Total bytes received */
        QVector<qint32> totalBytes;
    };
    
    /**
     * \brief The raw trace object
     */
    RawTrace m_trace;

    /**
     * \brief Pointer to the on/off download task object
     */
    OnoffDownloadTask *m_task;

private:
    virtual void run();

public:
    /**
     * \brief Initialize the on/off download client
     * \param localAddr Client's IPv4 address
     * \param serverAddr Server's IPv4 address
     * \param task Pointer to the input task object
     */
    OnoffDownloadClient(const QString &localAddr, 
                        const QString &serverAddr, OnoffDownloadTask *task);

    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

#endif /* ONOFF_DOWNLOAD_CLIENT_H */
