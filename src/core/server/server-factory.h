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

#ifndef SERVER_FACTORY_H
#define SERVER_FACTORY_H

#include "server.h"
#include "../task/task.h"

/**
 * \brief Factory for creating servers objects
 * \ingroup Factories
 */

class ServerFactory
{
public:
    /**
     * \brief Create a server object
     * \param type The type of the concrete client
     * \param addr IP address of the server
     * \param port Port of the server
     * \return The pointer to the created server object. If type is invalid, 
     *         a NULL pointer is returnd.
     * \note ServerFactory will not return a null client, make sure that
     *       code checking for NULL pointers are available.
     */
    static Server *create(Task::Type type, const QHostAddress &addr, 
        quint16 port);
};

#endif /* SERVER_FACTORY_H */

