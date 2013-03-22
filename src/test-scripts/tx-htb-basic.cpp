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
 * \file tx-htb-basic.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * tx-htb-basic <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */


/**
 * \brief Execute test
 * \param bulkUsers The number of bulk download users
 * \param nThreads The size of the threads array
 * \param threads An array that contains the number of threads in test
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
void execTest(ConsoleApplication &app, 
              int bulkUsers, int nThreads, int *threads, // Test options
              bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs;
    
    for (int index = 0; index < nThreads; index++)
    {
        int n = threads[index]; // n threads
        
        if (printDetail)
        {
            printf("\nPass %d / %d, %d Threads\n", index + 1, nThreads, n);
            printf("------------------------------------------\n");
        }
        
        // Setup scenario
        Scenario s(12345, 10); // seed & length
        
        s.addUser("Realtime User");
        s.addTask("Realtime User", new TcpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 10, 10");

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
        
        // Wait for a while
        Utils::msleep(1000);
    }
    
    if (printSummary)
    {
        printf("\n===========");
        for (int index = 0; index < nThreads; index++) { printf("====="); }
        printf("\nThreads   |");
        for (int index = 0; index < nThreads; index++) { printf("%5d", threads[index]); }
        printf("\n----------+");
        for (int index = 0; index < nThreads; index++) { printf("-----"); }
        printf("\nMin Delay |");
        for (int index = 0; index < nThreads; index++) { printf("%5d", mins[index]); }
        printf("\nMax Delay |");
        for (int index = 0; index < nThreads; index++) { printf("%5d", maxs[index]); }
        printf("\nAvg Delay |");
        for (int index = 0; index < nThreads; index++) { printf("%5d", avgs[index]); }
        printf("\n===========");
        for (int index = 0; index < nThreads; index++) { printf("====="); }
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
    for (int test_index = 0; test_index < 2; test_index++)
    {
        // Setup Router
        if (test_index == 0) // In test one, fifo queues are applied as the default qdisc
        {
            terminal.enter("tc qdisc show\n");
        }
        else // In test two, use htb
        {
            terminal.enter("tc qdisc add dev eth0 root handle 1: htb default 40\n");
            terminal.enter("tc class add dev eth0 parent 1: classid 1:1 htb rate 240kbit\n");
            terminal.enter("tc class add dev eth0 parent 1:1 classid 1:10 htb rate 60kbit ceil 240kbit\n");
            terminal.enter("tc class add dev eth0 parent 1:1 classid 1:20 htb rate 60kbit ceil 240kbit\n");
            terminal.enter("tc class add dev eth0 parent 1:1 classid 1:30 htb rate 60kbit ceil 240kbit\n");
            terminal.enter("tc class add dev eth0 parent 1:1 classid 1:40 htb rate 60kbit ceil 240kbit\n");
            terminal.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 handle 10 fw flowid 1:10\n");
            terminal.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 handle 20 fw flowid 1:20\n");
            terminal.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 handle 30 fw flowid 1:30\n");
            terminal.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 handle 40 fw flowid 1:40\n");
            terminal.enter("iptables -t mangle -A FORWARD -s 172.16.0.16 -j MARK --set-mark 10\n");
            terminal.enter("iptables -t mangle -A FORWARD -s 172.16.0.17 -j MARK --set-mark 20\n");
            terminal.enter("iptables -t mangle -A FORWARD -s 172.16.0.18 -j MARK --set-mark 30\n");
            terminal.enter("iptables -t mangle -A FORWARD -s 172.16.0.19 -j MARK --set-mark 40\n");
        }

        // Execute Test
        int threads[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        execTest(app, 3, 15, threads, true, true);

        // Reset router
        if (test_index == 1)
        {
            terminal.enter("tc qdisc del dev eth0 root\n");
            terminal.enter("iptables -t mangle -F FORWARD\n");
        }
    }
    emulator.reset();
    terminal.close();

    // Exit
    return 0;
}

