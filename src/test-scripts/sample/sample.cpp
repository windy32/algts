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

#include "../../core/core.h"

/**
 * Client application sample
 *
 * A clinet application is an application that runs on the client host
 * in the algts testbed.
 *
 * When the testbed has been set up (which means that you've got a client host
 * connected to a router, the router connected to a server host, and a
 * server daemon as well as a emulator daemon are running on the server host), 
 * you can script a scenario and then execute the test.
 *
 * In this sample, there're two users, Harry and Sally. Harry has a bulk download task
 * running on port 80, and a on/off download task running on port 80. Sally has a tcp echo
 * task running on port 23, and a udp echo task running on port 4000.
 *
 * Each task may have some specific attributes, e.g., for a bulk download task, you can
 * specify the max bytes to get, and the max rate server send data. 
 *
 * A scenario consists of three parts: users and tasks, a seed and a length.
 *
 * The seed is used while generating numbers from an random variable attribute. This is
 * useful when you just want to run the test again, or run the with same attributes but 
 * different concrete parameters. Although seeding in the application layer cannot
 * guarantee exactly the same result, compared to discrete event simulators like ns2/ns3, 
 * it's helpful getting results relatively stable.
 *
 * The length describes the max length of the test, which applies to all tasks in the scenario.
 *
 * A scenario object has nothing to do with address of users and servers.
 * To run a test in application layer, however, concrete ip addresses are necessary as they
 * should appear in ip packets.
 *
 * An ClientApplication object process the information that's necessary for a test, 
 * its constructor tasks 3 parameters:
 *  - Avaliable local ip addresses
 *  - Address of the server daemon
 *  - Port of the server daemon
 * \code
 *  ClientApplication app(localAddrs, QHostAddress("10.0.0.1"), 3201)
 * \endcode
 * In the code above, server daemon's address and port is set to 10.0.0.1:3201.
 *
 * Finally, call app.exec(Scenario *s) to run the test. If the number of users is
 * larger than the number of local addresses available (e.g., when Harry, Sally and Tom
 * share the link but only 10.0.0.8 and 10.0.0.9 are available), an error message
 * will show up, and test will not continue.
 * 
 * It's common to create a terminal object and a emulator object, and put setting-up scripts
 * before running the test. If neither one has been done, the router will forward packets
 * using the default FIFO queue, and no extra delay or rate limit that
 * emulate the properties of wide area networks will be generated.
 *
 * Usage: app <local-address-range> <daemon-address> <daemon-port>\n"
 */

int main(int argc, char *argv[])
{
    // Start console application and check arguments
    ConsoleApplication app(argc, argv);
    
    // Enable logging
    Log::enable(Log::LOG_LEVEL_INFO);
    
    // Enable tracing
    TextTrace::enable(argv[0]);

    // Setup router
    TelnetTerminal terminal("172.16.0.1");
    terminal.enter("root\n");
    terminal.enter("admin\n");
    terminal.enter("tc qdisc show\n");
    terminal.close();
    
    // Setup emulator
    NetemEmulator emulator("10.0.0.1", 3201);
    emulator.setParam("TxRate", "200kbps");
    emulator.setParam("RxRate", "2000kbps");
    emulator.setParam("TxDelay", "20ms");
    emulator.setParam("RxDelay", "20ms");
    emulator.commit();
    
    // Setup scenario
    Scenario s(12345, 60); // seed & length
    s.addUser("Harry");
    s.addUser("Sally");

    s.addTask("Harry", new BulkDownloadTask(80));
    s.task()->setAttribute("MaxBytes", "1MB");
    s.task()->setAttribute("MaxRate", "1Mbps");
    
    s.addTask("Harry", new BulkUploadTask(80));
    s.task()->setAttribute("MaxBytes", "INFINITE");
    s.task()->setAttribute("MaxRate", "INFINITE");

    s.addTask("Harry", new OnoffDownloadTask(80));
    s.task()->setAttribute("OnTime", "Uniform 500, 10000");
    s.task()->setAttribute("OffTime", "Exponential 2000");
    s.task()->setAttribute("MaxRate", "INFINITE");
    s.task()->setAttribute("PacketSize", "1200B");
    s.task()->setAttribute("RequestSize", "50B");
    
    s.addTask("Sally", new TcpEchoTask(23));
    s.task()->setAttribute("InputSize", "Uniform 8, 8");
    s.task()->setAttribute("EchoSize", "Exponential 20, 1000");
    s.task()->setAttribute("Interval", "Pareto 500, 1.5, 10000");
    
    s.addTask("Sally", new AsyncUdpEchoTask(4000));
    s.task()->setAttribute("InputSize", "Uniform 10, 100");
    s.task()->setAttribute("EchoSize", "Uniform 10, 500");
    s.task()->setAttribute("Interval", "Pareto 600, 1.4");
    
    // Execute
    app.exec(&s);
    
    return 0;
}
