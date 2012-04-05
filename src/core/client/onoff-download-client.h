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
 * Trace Format of On/Off Download Task
 *
 * Regular trace items for rating gui
 *
 *  - vector [Regular RxRate: byte/s, int32]
 *  - vector [Regular MaxRxRate: byte/s, int32]
 *  - vector [Regular Active, BOOL, int32]
 *
 * Irregular trace items for script outputs
 *
 * vector of vector {period, index, time(ms), newBytes, totalBytes}, e.g.,
 * 
 *    0, 0, 11, 0, 0           <- connecting to server (first line in the on period period)
 *    0, 1, 30, 1000, 1000     <- first data packet receive in the on period
 *    0, 2, 99, 1450, 2450
 *    0, 3, 127, 1450, 3900
 *    0, 4, 183, 1000, 4900
 *    0, 5, 227, 1000, 5900
 *    0, 6, 472, 1000, 6900
 *    ...
 *    0, 71, 5532, 1000, 59900 <- last data packet received in the on period
 *                             <- sleep during the off period
 *
 *    1, 0, 16882, 0, 0 // connecting to server (first line in the next on period)
 *    ...
 */

class OnoffDownloadClient : public Client
{
private:
    struct RawTrace
    {
        QVector<qint32> period;
        QVector<qint32> index;
        QVector<qint32> time;
        QVector<qint32> newBytes;
        QVector<qint32> totalBytes;
    } m_trace;

	OnoffDownloadTask *m_task;

private:
    virtual void run();

public:
    OnoffDownloadClient(const QString &localAddr, const QString &serverAddr);

    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

#endif /* ONOFF_DOWNLOAD_CLIENT_H */
