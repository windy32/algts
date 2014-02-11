// Copyright (C) 2012-2014 Fengyu Gao (feng32tc@gmail.com)
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
 * \file htb-buffer-size.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * htb-buffer-size <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */

/**
 * \brief Execute test
 * \param app The Console Application object
 * \param bandwidth Bandwidth of the emulator
 * \param sessions Number of download sessions
 * \param ceil Init value of the ceil rate
 * \param nPass Specify how many times each test is executed
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
void execTest(ConsoleApplication &app, 
             int bandwidth, int sessions, int ceil, int nPass, // Search options
             bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs;

    for (int index = 0; index < nPass; index++)
    {
        if (printDetail)
        {
            printf("\nPass %d / %d\n", index + 1, nPass);
            printf("------------------------------------------\n");
        }
        
        // Setup scenario
        Scenario s(12345, 10); // seed & length
        s.addUser("RTT Observer");
        s.addTask("RTT Observer", new TcpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 10, 10");

        s.addUser("Downloader");
        for (int i = 0; i < sessions; i++)
        {
            s.addTask("Downloader", new BulkDownloadTask(80));
            s.task()->setAttribute("MaxBytes", "INFINITE");
            s.task()->setAttribute("MaxRate", "INFINITE");
        }
            
        // Execute
        app.exec(&s);

        // Collect statistics
        RawTraceItem rti;
        app.exportRawTrace("RTT Observer", 0, rti); // Focus only on rt user
        
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
        printf("\nBandwidth = %d kbps, threads = %d, ceil = %d%%", bandwidth, sessions, ceil);
        printf("\n========================================");
        printf("\nPass | Min Delay | Max Delay | Avg Delay");
        printf("\n-----+----------------------------------");
        for (int index = 0; index < nPass; index++)
        {
            printf("\n%4d | %9d | %9d | %9d", index + 1, mins[index], maxs[index], avgs[index]);
        }
        printf("\n========================================\n\n");
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

    // Setup router
    SshTerminal terminal("-p voyage ssh root@172.16.0.1"); // use sshpass
    terminal.start();

    // Exec tests
    int bandwidth = 8000; // 8 Mbps
    int pct = 90; // 10% reserved
    int sessions[8] = { 2, 4, 6, 8, 12, 16, 24, 32 };
    int packets[6] = { -1, 4, 16, 64, 256, 1024 };
    
    for (int i = 0; i < 6; i++)
    {
    for (int j = 0; j < 8; j++)
    {
        // Setup emulator
        emulator.setParam("Algorithm", "htb");
        emulator.setParam("FairQueue", "off");
        emulator.setParam("TxRate", QString("%1kbps").arg(bandwidth / 4));
        emulator.setParam("RxRate", QString("%1kbps").arg(bandwidth));
        emulator.commit();

        // Setup Router
        int ceil = bandwidth * pct / 100;
        terminal.enter("tc qdisc add dev eth1 root handle 1: htb default 20\n");
        terminal.enter(QString(
            "tc class add dev eth1 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(ceil).arg(ceil));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:10 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(ceil / 2).arg(ceil));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:20 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(ceil / 2).arg(ceil));
        if (i > 0) // 0: do not attach a pfifo manually
        {
            terminal.enter(QString(
                "tc qdisc add dev eth1 parent 1:10 handle 10: pfifo limit %1\n").arg(packets[i]));
            terminal.enter(QString(
                "tc qdisc add dev eth1 parent 1:20 handle 20: pfifo limit %1\n").arg(packets[i]));
        }
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.16 flowid 1:10\n");
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.17 flowid 1:20\n");

        // Execute Test
        execTest(app, bandwidth, sessions[j], pct, 50, true, true);

        // Reset emulator
        emulator.reset();

        // Reset router
        terminal.enter("tc qdisc del dev eth1 root\n");
    }
    }
    terminal.close();

    // Exit
    return 0;
}

