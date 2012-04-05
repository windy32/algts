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

class ConsoleApplication : public CoreApplication
{
private:
    bool parseAddr(const QString &str, QHostAddress &addr);
    bool parsePort(const QString &str, quint16 &port);
    bool parseAddrRange(const QString &str, QList<QHostAddress> &addrRange);

public:
    ConsoleApplication(int argc, char *argv[]);
};

#endif /* CORE_APPLICATION_H */
