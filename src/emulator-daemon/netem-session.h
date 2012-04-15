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

#ifndef NETEM_SESSION_H
#define NETEM_SESSION_H

#include "../core/common.h"

/**
 * \brief The netem emulator daemon session
 * \ingroup EmulatorDaemon
 * \note After a session is established, the EmulatorDaemonSession first
 *       receives the whole request, and then checks the emulatorName field.
 *       If the value of this fild is "NetEm", process of the request continues
 *       in NetemSession::parse.\n\n
 *       When NetemSession::parse is called, the remaining part of the request
 *       (the command field and the following) is still stored in the buffer
 *       of the QTcpSocket.
 *       
 */
class NetemSession
{
private:
    bool execCommand(const QString &input, const QString &expectedOutput);

    bool execCommit(QMap<QString, QString> &params);
    bool execReset();
    
    //QTcpSocket *socket, QMap<QString, QString> &params);
    
private:
    bool parseCommit(QTcpSocket *socket, QMap<QString, QString> &params);

public:
    /**
     * \brief Parse the NetEm emulator specific part of the request, and execute
     *        the emulation requests.
     * \param socket The socket of the session
     * \param params The emulation parameters to be updated
     */
    void parse(QTcpSocket *socket, QMap<QString, QString> &params);
};

#endif /* NETEM_SESSION */

