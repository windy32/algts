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

#ifndef TELNET_TERMINAL_H
#define TELNET_TERMINAL_H
 
#include "terminal.h"

/**
 * \brief The telnet terminal
 * \ingroup Terminals
 */
class TelnetTerminal : public Terminal
{
public:
    /**
     * \brief Initialize the tenlet terminal
     * \param addr The address of remote host
     *
     * The command line starting the ssh terminal would be
     * \code
     * telnet <addr>
     * \endcode
     */
    TelnetTerminal(const QString &addr);
};

#endif /* TELNET_TERMINAL_H */
