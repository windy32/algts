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

#include "../core/core.h"

/**
 * \file tcp-echo.cpp
 * \ingroup Scripts
 * \brief Test script for tcp echo task
 *
 * Usage:
 * \code
 * tcp-echo <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 */

/**
 * \brief The entry point of the script
 */
int main(int argc, char *argv[])
{
    // Enable logging and tracing
    Log::enable(Log::LOG_LEVEL_DEBUG);
    TextTrace::enable(argv[0]);
    
    // Start console application and check arguments
    ConsoleApplication app(argc, argv);
    
    // Setup scenario
    LOG_DEBUG("Setting up scenaio");
    
    Scenario s(12345, 30); // seed & length
    s.addUser("Harry");

    s.addTask("Harry", new TcpEchoTask(23));
    s.task()->setAttribute("InputSize", "Uniform 8, 8");
    s.task()->setAttribute("EchoSize", "Exponential 20, 1000");
    s.task()->setAttribute("Interval", "Pareto 500, 1.5, 10000");
    
    // Execute
    app.exec(&s);
    
    return 0;
}

