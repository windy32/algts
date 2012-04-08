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

#ifndef TERMINAL_H
#define TERMINAL_H

#include "../common.h"

/**
 * \brief The base class of all terminals
 * \note Concrete terminal classes should explicitly initialize the m_program
 *       and the m_arguments members in constructor
 *
 * Terminals are typically used for setting up routers.
 * \code
 * // Setup router
 * TelnetTerminal terminal("172.16.0.1");
 * terminal.enter("root\n");
 * terminal.enter("admin\n");
 * terminal.enter("tc qdisc add dev eth0 root htb ...\n");
 * ...
 * terminal.close();
 * \endcode
 * \see TelnetTerminal, SshTerminal
 */
class Terminal
{
protected:
    //qint32 m_responseDelay;
    //qint32 m_response;
    
    /**
     * \brief The process object for the terminal
     */
    QProcess *m_process;
    /**
     * \brief The executable file name of the terminal
     */
    QString m_program;
    /**
     * \brief Arguments used starting the terminal process
     */
    QStringList m_arguments;
    
protected:
    /*
     **
     * The function should be called each time user enters a character
     * or a line, and a response is expected. For example, when user
     * logged in and enter "pwd\n", the expected response may be 
     * "/tmp/home/root\n # ", which is the current working path
     * followed by a new line and then a prompt string "# ".
     * 
     * The response may be huge in some cases (e.g., when executing find), 
     * where a single call is not enough to receive the whole response.
     *
     * In this function, it waits at most 300ms (m_responseDelay) for the
     * first piece of response. If no response is received (e.g., entering
     * password in Linux), it returns direct.
     *
     * When first piece of response is returned, it waits at most 
     * 100ms (m_
     *
    void waitForResponse();*/
    
public:
    /**
     * \brief Initialize the terminal
     */
    Terminal();

    /**
     * \brief Start the terminal process
     * \return Returns true if process started successfully, and false otherwise
     */
    virtual bool start();
    /**
     * \brief Send something to the terminal process' regular input
       \param input The content to enter
     */
    virtual void enter(const QString &input);
    /**
     * \brief Close the terminal process
     */
    virtual void close();
};

#endif /* TERMINAL_H */
