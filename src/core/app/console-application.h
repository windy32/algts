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

#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "core-application.h"

/**
 * \brief The console application.
 * \ingroup Applications
 *
 * This class is designed for convenience in scripting. Here's a mininal test
 * script without the ConsoleApplication.
 *
 * \code
 * int main(int argc, char *argv[])
 * {
 *     QList<QHostAddress> localAddrs;
 *     QHostAddress serverDaemonAddr;
 *     quint16 serverDaemonPort;
 *
 *     // Either set fixed address and port, or dynamically get desired address
 *     // and port from the command line
 *     //
 *     // dozens of code here...
 *
 *     // Start core application
 *     CoreApplication app(localAddrs, serverDaemonAddr, serverDaemonPort);
 *     
 *     // Setup scenario
 *     Scenario s(12345, 60); // seed & length
 *     s.addUser("Harry");
 *     ...
 *
 *     // Execute
 *     app.exec(&s);
 *
 *     return 0;
 * }
 * \endcode
 *
 * Now, with ConsoleApplication, a derived class of CoreApplication, parsing of
 * command-line parameters can be finished in a line.
 *
 * \code
 * int main(int argc, char *argv[])
 * {
 *     // Start console application
 *     CoreApplication app(argc, argv);
 *     
 *     // Setup scenario
 *     Scenario s(12345, 60); // seed & length
 *     s.addUser("Harry");
 *     ...
 *
 *     // Execute
 *     app.exec(&s);
 *
 *     return 0;
 * }
 * \endcode
 *
 * **Command line usage**
 * \code
 * app <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * **Command line example**
 * \code
 * app 172.16.0.8/29 10.0.0.1 3201
 * \endcode
 *
 * \see CoreApplication
 */
class ConsoleApplication : public CoreApplication
{
private:
    bool parseAddr(const QString &str, QHostAddress &addr);
    bool parsePort(const QString &str, quint16 &port);
    bool parseAddrRange(const QString &str, QList<QHostAddress> &addrRange);

public:
    /**
     * \brief Initialize the console application.
     * \param argc The number of command line argumnets
     * \param argv The content of command line arguments
     */
    ConsoleApplication(int argc, char *argv[]);
};

#endif /* CORE_APPLICATION_H */
