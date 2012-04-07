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

#ifndef Q_BOUND_TCP_SOCKET_H
#define Q_BOUND_TCP_SOCKET_H

#include <QTcpSocket>

/**
 * \brief The QBoundTcpSocket class adds binding support
 *        to tcp client sockets.
 *
 * In Qt 4, it's impossible to bind a tcp client socket to a specific port or
 * ip address.
 *
 * This feature has been added to Qt 5, however with version 4, it's still
 * possible to do binding while getting convenience from the Qt framework.
 * 
 * \note Due to usage of native api, the class currently not supports platforms
 *       other than Linux. If you are familiar with native programming
 *       interfaces like winsock, porting the code to other operations systems
 *       may be just a piece of cake.\n\n
 *       A QBoundTcpSocket object will bind and connect to the server at the
 *       same time, because the only way to work around is to create a socket,
 *       bind, connect to server with native API from the OS, and then pass the
 *       socket to QTcpSocket::setSocketDescriptor with a connected state.
 */

class QBoundTcpSocket : public QTcpSocket
{
public:
    /**
     * \brief Bind the socket and then connect to server
     * \param localAddr Local IP address (e.g., "10.0.0.10"). A value of
     *                  "0.0.0.0" indicates that the default ip address will be
     *                  used.
     * \param localPort Local port (e.g., 12345). A value of 0 indicates that
     *                  any port is acceptable.
     * \param serverAddr IP address of the server (e.g., "10.0.0.1")
     * \param serverPort Port of the server (e.g., 80)
     *
     * \return The return value is true when both binding and connecting 
     *         are successful, and false otherwise.
     *
     * \note Before calling, the socket must be in the unconnected state, 
     *       otherwise a value of false is returned.
     * 
     * Typical usage:
     * \code
     * QBoundTcpSocket socket;
     * if (socket.bindAndConnect("192.168.1.10", 0, "10.0.0.1", 80))
     * {
     *     // Now bound and connected
     * }
     * else
     * {
     *     qDebug() << "An error occurred in QBoundTcpSocket::bindAndConnect";
     * }
     * \endcode
     */
     bool bindAndConnect(const QString &localAddr, quint16 localPort, 
                         const QString &serverAddr, quint16 serverPort);
};

#endif /* Q_BOUND_TCP_SOCKET_H */

