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

#ifndef EMULATOR_DAEMON_SESSION_H
#define EMULATOR_DAEMON_SESSION_H

#include "../core/common.h"

/**
 * \brief The emulator daemon session
 * \ingroup EmulatorDaemon
 */
class EmulatorDaemonSession : public QThread
{
private:
    QTcpSocket *m_socket;
    QMap<QString, QString> &m_params;

protected:
    /**
     * \brief The entry point of the thread function
     */
    virtual void run();
    
public:
    /**
     * \brief Initialize the emulator daemon session
     * \param socket The socket for the new connection
     * \param params The emulation property table to be updated
     */
    EmulatorDaemonSession(QTcpSocket *socket, QMap<QString, QString> &params);
};

#endif /* EMULATOR_DAEMON_SESSION_H */
