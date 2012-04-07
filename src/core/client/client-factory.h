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

#ifndef CLIENT_FACTORY_H
#define CLIENT_FACTORY_H

#include "client.h"

/**
 * \brief Factory for creating client objects
 */

class ClientFactory
{
public:
    /**
     * \brief Create a client object
     * \param type The type of the concrete client
     * \param localAddr IP address of the client
     * \param serverAddr IP address of the server
     * \return The pointer to the created client object. If type is invalid, 
     *         a NULL pointer is returnd.
     * \note ClientFactory will not return a null client, make sure that
     *       code checking for NULL pointers are available.
     */
    static Client *create(Task::Type type, const QString &localAddr, 
                          const QString &serverAddr);
};

#endif /* CLIENT_FACTORY_H */

