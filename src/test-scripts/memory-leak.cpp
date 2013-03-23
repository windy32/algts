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
 * \file memory-leak.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * memory-leak <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */

/**
 * \brief The entry point of the sample
 */
int main(int argc, char *argv[])
{
    // Start console application and check arguments
    ConsoleApplication app(argc, argv);
    
    // Enable logging
    Log::enable(Log::LOG_LEVEL_INFO);
    
    // Setup emulator
    BasicEmulator emulator("10.0.0.1", 3201);

    // Setup emulator
    emulator.setParam("Algorithm", "htb");
    emulator.setParam("FairQueue", "on");
    emulator.setParam("TxRate", "1000kbps");
    emulator.setParam("RxRate", "2000kbps");
    emulator.commit();

    // Setup router
    SshTerminal terminal("-p voyage ssh root@172.16.0.1"); // use sshpass
    terminal.start();

    terminal.enter("tc qdisc add dev eth1 root handle 1: htb default 40\n");
    terminal.enter(QString("tc class add dev eth1 parent 1: classid 1:1 htb rate 1800kbit ceil 1800kbit quantum 1540\n"));
    terminal.enter(QString("tc class add dev eth1 parent 1:1 classid 1:10 htb rate 400kbit ceil 1800kbit\n"));
    terminal.enter(QString("tc class add dev eth1 parent 1:1 classid 1:20 htb rate 400kbit ceil 1800kbit\n"));
    terminal.enter(QString("tc class add dev eth1 parent 1:1 classid 1:30 htb rate 400kbit ceil 1800kbit\n"));
    terminal.enter(QString("tc class add dev eth1 parent 1:1 classid 1:40 htb rate 400kbit ceil 1800kbit\n"));
    terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.16 flowid 1:10\n");
    terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.17 flowid 1:20\n");
    terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.18 flowid 1:30\n");
    terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.19 flowid 1:40\n");

    for (int i = 0; i < 100; i++)
    {
        printf("Pass %d / %d\n", i + 1, 100);
        
        // Setup scenario
        Scenario s(12345, 5); // seed & length
        
        s.addUser("Realtime User");
        s.addTask("Realtime User", new TcpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 10, 10");

        for (int i = 0; i < 3; i++)
        {
            char username[32];
            sprintf(username, "Bulk Download User %d", i + 1);
            s.addUser(username);

            for (int j = 0; j < 20; j++)
            {
                s.addTask(username, new BulkDownloadTask(80));
                s.task()->setAttribute("MaxBytes", "INFINITE");
                s.task()->setAttribute("MaxRate", "INFINITE");
            }
        }
        
        // Execute
        app.exec(&s);
    }
    
    // Reset
    emulator.reset();
    terminal.enter("tc qdisc del dev eth1 root\n");
    terminal.close();

    // Exit
    return 0;
}

