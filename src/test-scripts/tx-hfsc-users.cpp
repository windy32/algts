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

#include "../core/core.h"

/**
 * \file tx-hfsc-users.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * tx-hfsc-users <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */


/**
 * \brief Execute test
 * \param bulkUsers The number of bulk download users
 * \param minThreads Min number of threads
 * \param maxThreads Max number of threads
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
void execTest(ConsoleApplication &app, 
              int bulkUsers, int minThreads, int maxThreads, // Test options
              bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs;
    
    for (int n = minThreads; n <= maxThreads; n++) // n threads
    {
        if (printDetail)
        {
            printf("\nPass %d / %d, %d Threads\n", 
                    n - minThreads + 1, maxThreads - minThreads + 1, n);
            printf("------------------------------------------\n");
        }
        
        // Setup scenario
        Scenario s(12345, 20); // seed & length
        
        s.addUser("Realtime User");
        s.addTask("Realtime User", new TcpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 20, 20");

        for (int i = 0; i < bulkUsers; i++)
        {
            char username[32];
            sprintf(username, "Bulk Upload User %d", i + 1);
            s.addUser(username);

            for (int j = 0; j < n; j++)
            {
                s.addTask(username, new BulkUploadTask(80));
                s.task()->setAttribute("MaxBytes", "INFINITE");
                s.task()->setAttribute("MaxRate", "INFINITE");
            }
        }
        
        // Execute
        app.exec(&s);

        // Collect statistics
        RawTraceItem rti;
        app.exportRawTrace("Realtime User", 0, rti); // Focus only on rt user
        
        int nSamples = rti["Delay"].size();
        int min = 10000; // init to 10 sec
        int max = 0; // 0 ms
        int avg = 0; // 0 ms
        int count[21]; // 0 - 19ms, 20 - 39ms, ..., 380 - 399ms, 400+ ms

        for (int i = 0; i < 21; i++) { count[i] = 0; }

        for (int i = 0; i < nSamples; i++)
        {
            min = qMin(min, rti["Delay"][i]);
            max = qMax(max, rti["Delay"][i]);
            avg += rti["Delay"][i];

            if (rti["Delay"][i] < 400)
            {
                count[rti["Delay"][i] / 20] += 1;
            }
            else
            {
                count[20] += 1;
            }
        }
        avg /= nSamples;

        mins.append(min);
        maxs.append(max);
        avgs.append(avg);

        // Output
        if (printDetail)
        {
            printf("%d Samples\n", nSamples);
            printf("Min delay: %dms, Max delay: %dms, Avg delay: %dms\n", min, max, avg);

            for(int i = 0; i < 20; i++)
            {
                printf("%4d", (i + 1) * 20);
            }
            printf(" 400+\n");
            
            for (int i = 0; i < 21; i++)
            {
                printf("%4d", count[i]);
            }
            printf("\n");
        }
    }
    
    if (printSummary)
    {
        printf("\n===========");
        for (int n = minThreads; n <= maxThreads; n++) { printf("====="); }
        printf("\nThreads   |");
        for (int n = minThreads; n <= maxThreads; n++) { printf("%5d", n); }
        printf("\n----------+");
        for (int n = minThreads; n <= maxThreads; n++) { printf("-----"); }
        printf("\nMin Delay |");
        for (int n = minThreads; n <= maxThreads; n++) { printf("%5d", mins[n - minThreads]); }
        printf("\nMax Delay |");
        for (int n = minThreads; n <= maxThreads; n++) { printf("%5d", maxs[n - minThreads]); }
        printf("\nAvg Delay |");
        for (int n = minThreads; n <= maxThreads; n++) { printf("%5d", avgs[n - minThreads]); }
        printf("\n===========");
        for (int n = minThreads; n <= maxThreads; n++) { printf("====="); }
        printf("\n\n");
    }
}

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
    emulator.setParam("Algorithm", "htb");
    emulator.setParam("FairQueue", "off");
    emulator.setParam("TxRate", "256kbps");
    emulator.setParam("RxRate", "2000kbps");
    emulator.commit();

    // Setup router
    SshTerminal terminal("-p voyage ssh root@172.16.0.1"); // use sshpass
    terminal.start();

    // Exec tests
    for (int bulkUsers = 1; bulkUsers <= 9; bulkUsers++)
    {
        // Setup Router
        terminal.enter(QString(
            "tc qdisc add dev eth0 root handle 1: hfsc default %1\n")
            .arg((bulkUsers + 1) * 10));
        terminal.enter(
            "tc class add dev eth0 parent 1: classid 1:1 hfsc ls rate 240kbit ul rate 240kbit\n");
        for (int i = 1; i <= bulkUsers + 1; i++)
        {
            terminal.enter(QString(
                "tc class add dev eth0 parent 1:1 classid 1:%1 hfsc sc umax 100b dmax 5ms rate %2kbit\n")
                .arg(i * 10).arg(240 / (bulkUsers + 1)));
            terminal.enter(QString(
                "tc filter add dev eth0 parent 1: protocol ip prio 1 handle %1 fw flowid 1:%1\n")
                .arg(i * 10));
            terminal.enter(QString(
                "iptables -t mangle -A FORWARD -s 172.16.0.%1 -j MARK --set-mark %2\n")
                .arg(16 + i - 1).arg(i * 10));
        }

        // Execute Test
        printf("\nTest %d: %d users\n", bulkUsers, bulkUsers);
        execTest(app, bulkUsers, 1, 16, false, true);

        // Reset router
        terminal.enter("tc qdisc del dev eth0 root\n");
        terminal.enter("iptables -t mangle -F FORWARD\n");
    }
    emulator.reset();
    terminal.close();

    // Exit
    return 0;
}

