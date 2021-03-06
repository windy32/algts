// Copyright (C) 2012-2013 Fengyu Gao (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
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

#ifndef SSH_TERMINAL_H
#define SSH_TERMINAL_H

#include "terminal.h"

/**
 * \brief The ssh terminal
 * \ingroup Terminals
 */
class SshTerminal : public Terminal
{
public:
    /**
     * \brief Initialize the ssh terminal
     * \param addr The address of remote host
     * 
     * The command line starting the ssh terminal would be
     * \code
     * ssh <addr>
     * \endcode
     */
    SshTerminal(const QString &addr);

    virtual void enter(const QString &input);
};

#endif /* SSH_TERMINAL_H */

