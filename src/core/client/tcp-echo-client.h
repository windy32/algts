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

#ifndef TCP_ECHO_CLIENT_H
#define TCP_ECHO_CLIENT_H

#include "client.h"
#include "../task/tcp-echo-task.h"

/**
 * \brief The tcp echo task client
 * \ingroup Clients
 *
 * Regular trace format:
 *  - vector<int32> Delay (ms)
 *  - vector<int32> Active (BOOL)
 *
 * Text trace format:
 *  - index
 *  - time (ms)
 *  - delay (ms)
 *
 * An example of text trace output:
 * \code
 * 0, 30, 63
 * 1, 99, 43
 * 2, 127, 72
 * 3, 183, 255 (packets won't be lost in a tcp echo task, it just tasks longer)
 * 4, 227, 289 
 * 5, 472, 58
 * ...
 * \endcode
 */
class TcpEchoClient : public Client
{
private:
    struct RawTrace
    {
        QVector<qint32> index;
        QVector<qint32> time;
        QVector<qint32> delay;
    } m_trace;

	TcpEchoTask *m_task;

private:
	virtual void run();

public:
    /**
     * \brief Initialize the tcp echo client
     * \param localAddr Client's IPv4 address
     * \param serverAddr Server's IPv4 address
     */
    TcpEchoClient(const QString &localAddr, const QString &serverAddr);

    virtual void generateTextTrace(TextTraceItem &trace);
    virtual void generateRegularTrace(RegularTraceItem &trace, int seconds);
};

#endif /* TCP_ECHO_CLIENT_H */
