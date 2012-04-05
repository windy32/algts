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

#ifndef SERVER_H
#define SERVER_H

#include "../common.h"

class Server : public QThread
{
protected:
    QHostAddress m_addr;
    quint16 m_port;

protected:
    virtual void run() = 0;
    
public:
    Server(const QHostAddress &addr, quint16 port);
    virtual bool start(QString &description) = 0;
};

#endif /* SERVER_H */
