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
 * \file rx-htb-summary.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * rx-htb-summary <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */

/**
 * \brief Execute test
 * \param app The Console Application object
 * \param terminal The SSH Terminal object
 * \param ceil Init value of the ceil rate
 * \param targetDelay The test tries to keep the average delay under this value 
 * \param nPass Specify how many times each test is executed
 * \param bandwidth Bandwidth of the emulator
 * \param threads Number of download sessions of each bulk user
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
int calcCeilRate(ConsoleApplication &app, SshTerminal &terminal,
                 int ceil, int targetDelay, int nPass, // Search Options
                 int bandwidth, int threads, // Scenario Options
                 bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs;
    QVector<int> ceils, avgCeils;

    // At the beginning, only one ceil rate is available
    ceils.append(ceil);
    avgCeils.append(ceil);
    int sumCeil = ceil;
    int avgCeil = ceil;
    
    for (int index = 0; index < nPass; index++)
    {
        if (printDetail)
        {
            printf("\nPass %d / %d\n", index + 1, nPass);
            printf("------------------------------------------\n");
        }
        
        // Setup Router
        terminal.enter("tc qdisc add dev eth1 root handle 1: htb default 40\n");
        terminal.enter(QString(
            "tc class add dev eth1 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit quantum 1540\n")
            .arg(ceil * bandwidth / 100).arg(ceil * bandwidth / 100));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:10 htb rate %1kbit ceil %2kbit\n")
            .arg(ceil * bandwidth / 100 / 4).arg(ceil * bandwidth / 100));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:20 htb rate %1kbit ceil %2kbit\n")
            .arg(ceil * bandwidth / 100 / 4).arg(ceil * bandwidth / 100));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:30 htb rate %1kbit ceil %2kbit\n")
            .arg(ceil * bandwidth / 100 / 4).arg(ceil * bandwidth / 100));
        terminal.enter(QString(
            "tc class add dev eth1 parent 1:1 classid 1:40 htb rate %1kbit ceil %2kbit\n")
            .arg(ceil * bandwidth / 100 / 4).arg(ceil * bandwidth / 100));
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.16 flowid 1:10\n");
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.17 flowid 1:20\n");
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.18 flowid 1:30\n");
        terminal.enter("tc filter add dev eth1 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.19 flowid 1:40\n");
        
        // Setup scenario
        Scenario s(12345, 10); // seed & length
        
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

            for (int j = 0; j < threads; j++)
            {
                s.addTask(username, new BulkDownloadTask(80));
                s.task()->setAttribute("MaxBytes", "INFINITE");
                s.task()->setAttribute("MaxRate", "INFINITE");
            }
        }
        
        // Execute
        app.exec(&s);

        // Reset router
        terminal.enter("tc qdisc del dev eth1 root\n");

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
            
            printf("AvgCeil = %d, NewCeil = %d, Result = %d ms, Target = %d ms\n", 
                avgCeil, ceil, avg, targetDelay);
        }

        // Adjust ceil rate
        double x = avg / (double)targetDelay;
        if (x < 0.5) x = 0.5;
        if (x > 2.0) x = 2.0;
        if (x < 1.0) // Interpolation with alpha = beta = 0.1
        {
            double k = (x - 0.5) / 0.5;
            ceil = (1 - k) * (ceil * 1.2) + k * ceil;
        }
        else
        {
            double k = (x - 1.0) / 1.0;
            ceil = (1 - k) * ceil + k * (ceil * 0.8);
        }
        
        // Calc Average
        sumCeil += ceil;
        avgCeil = sumCeil / (index + 2);
        avgCeils.append(avgCeil);
        ceils.append(ceil);

        // TODO: Fix memory leak in server daemon
        
        // Wait for a while
        Utils::msleep(1000);
    }
    
    if (printSummary)
    {
        printf("\nBandwidth = %d kbps, threads = %d", bandwidth, threads);
        printf("\n==============================================================");
        printf("\nPass | Min Delay | Max Delay | Avg Delay | New Ceil | Avg Ceil");
        printf("\n-----+--------------------------------------------------------");
        for (int index = 0; index < nPass; index++)
        {
            printf("\n%4d | %9d | %9d | %9d | %8d | %8d", 
                index + 1, mins[index], maxs[index], avgs[index], ceils[index], avgCeils[index]);
        }
        printf("\n==============================================================\n\n");
    }
    
    return avgCeil;
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
    for (int bandwidth = 1000; bandwidth <= 10000; bandwidth += 1000)
    {
        for (int threads = 1; threads <= 15; threads++)
        {
            // Setup emulator
            emulator.setParam("Algorithm", "htb");
            emulator.setParam("FairQueue", "on");
            emulator.setParam("TxRate", "1000kbps");
            emulator.setParam("RxRate", QString("%1kbps").arg(bandwidth));
            emulator.commit();

            // Execute Test
            calcCeilRate(app, terminal, 75, 80, 40, bandwidth, threads, true, true);

            // Reset emulator
            emulator.reset();
        }
    }
    terminal.close();

    // Exit
    return 0;
}

