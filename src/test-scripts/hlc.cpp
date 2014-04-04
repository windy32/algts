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
#include <math.h>

/**
 * \file hlc.cpp
 * \ingroup Scripts
 * \brief The "Hard limit CoDel" test script
 *
 * Usage:
 * \code
 * hlc <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */

/**
 * \brief Execute test
 * \param app The Console Application object
 * \param bandwidth Bandwidth of the emulator
 * \param sessions Number of download sessions
 * \param nPass Specify how many times each test is executed
 * \param desc Description
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
void execTest(ConsoleApplication &app, 
             int bandwidth, int sessions, int nPass, int rtt, // Search options
             bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs, rates;

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
        s.addTask("RTT Observer", new AsyncUdpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 20, 20");

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
        app.exportRawTrace("RTT Observer", 0, rti);
        
        int nSamples = rti["Delay"].size();
        int nValid = 0;
        int min = 10000; // init to 10 sec
        int max = 0; // 0 ms
        int avg = 0; // 0 ms
        int count[21]; // 0 - 19ms, 20 - 39ms, ..., 380 - 399ms, 400+ ms
        int rate = 0; // Throughput init to 0 KB/s

        for (int i = 0; i < 21; i++) { count[i] = 0; }

        for (int i = 0; i < nSamples; i++)
        {
            if (rti["Delay"][i] == -1) // packet lost
                continue;

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
            nValid += 1;
        }
        avg /= nValid;
        
        for (int i = 0; i < sessions; i++)
        {
            app.exportRawTrace("Downloader", i, rti);
            rate += rti["TotalBytes"].last() / 1024;
        }
        rate /= 10;
            
        mins.append(min);
        maxs.append(max);
        avgs.append(avg);
        rates.append(rate);

        // Output
        if (printDetail)
        {
            printf("%d Samples\n", nSamples);
            printf("Rate: %d KB/s, Min delay: %dms, Max delay: %dms, Avg delay: %dms\n", rate, min, max, avg);

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
        printf("\nRTT = %d ms, Bandwidth = %d kbps, threads = %d", rtt, bandwidth, sessions);
        printf("\n=====================================================");
        printf("\nPass | Min Delay | Max Delay | Avg Delay | Throughput");
        printf("\n-----+-----------------------------------------------");
        for (int index = 0; index < nPass; index++)
        {
            printf("\n%4d | %9d | %9d | %9d | %10d", index + 1, mins[index] - rtt, maxs[index] - rtt, avgs[index] - rtt, rates[index]);
        }
        printf("\n=====================================================\n\n");
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
    
    // Setup
    SshTerminal gateway("-p admin ssh root@192.168.1.1"); // use sshpass
    SshTerminal isp("-p admin ssh root@172.16.0.1");
    SshTerminal server("-p voyage ssh root@10.0.0.1");
    gateway.start();
    isp.start();
    server.start();

    // In OpenWRT, sch modules are not loaded automatically on startup
    // However, the server is running Voyage based on x86 Debian
    gateway.enter("insmod sch_htb\n");
    gateway.enter("insmod sch_codel\n");
    isp.enter("insmod sch_htb\n");
    isp.enter("insmod sch_codel\n");
    isp.enter("insmod sch_netem\n");

    // Exec tests
    int rtts[10] = { 10, 20, 40, 80, 120, 160, 240, 320, 400, 500 };
    int bandwidths[4] = { 1000, 4000, 16000, 64000 };
    int sessions[7] = { 1, 2, 4, 8, 16, 32, 64 };
    // Flow number cannot be too large, otherwise there would be a warning:
    // TCP: Possible SYN flooding on port 80. Dropping request.
    
    for (int algorithm = 2; algorithm < 3; algorithm++)
    {
    for (int r = 0; r < 10; r++) // rtt_index
    {
    for (int i = 0; i < 4; i++) // bandwidth_index
    {
    for (int j = 0; j < 7; j++) // session_index
    {
        int bw = bandwidths[i];
        int delay = rtts[r] / 2;
        int flows = sessions[j];
        int bytes = (int)(bw * 1000 / 8 * 0.05); // 50ms bfifo queue

        // Set AQM
        if (algorithm == 0) // fifo
        {
            gateway.enter("tc qdisc add dev eth1 root handle 1: htb default 1\n");
            gateway.enter(QString("tc class add dev eth1 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            gateway.enter(QString("tc qdisc add dev eth1 parent 1:1 handle 10: bfifo limit %1\n").arg(bytes));

            isp.enter("tc qdisc add dev eth0 root handle 1: htb default 1\n");
            isp.enter(QString("tc class add dev eth0 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            isp.enter(QString("tc qdisc add dev eth0 parent 1:1 handle 10: bfifo limit %1\n").arg(bytes));
        }
        else if (algorithm == 1) // codel
        {
            gateway.enter("tc qdisc add dev eth1 root handle 1: htb default 1\n");
            gateway.enter(QString("tc class add dev eth1 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            gateway.enter("tc qdisc add dev eth1 parent 1:1 handle 10: codel\n");

            isp.enter("tc qdisc add dev eth0 root handle 1: htb default 1\n");
            isp.enter(QString("tc class add dev eth0 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            isp.enter("tc qdisc add dev eth0 parent 1:1 handle 10: codel\n");
        }
        else if (algorithm == 2) // hard limit codel
        {
            gateway.enter("tc qdisc add dev eth1 root handle 1: htb default 1\n");
            gateway.enter(QString("tc class add dev eth1 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            gateway.enter("tc qdisc add dev eth1 parent 1:1 handle 10: codel maxdelay 50ms\n");

            isp.enter("tc qdisc add dev eth0 root handle 1: htb default 1\n");
            isp.enter(QString("tc class add dev eth0 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit\n").arg(bw).arg(bw));
            isp.enter("tc qdisc add dev eth0 parent 1:1 handle 10: codel maxdelay 50ms\n");
        }
        
        // Emulate RTT
        server.enter(QString("tc qdisc add dev eth1 root handle 1: netem delay %1ms\n").arg(delay));
        isp.enter(QString("tc qdisc add dev eth1 root handle 1: netem delay %1ms\n").arg(delay));

        // Execute Test
        execTest(app, bw, flows, 3, rtts[r], true, true);

        // Reset
        gateway.enter("tc qdisc del dev eth1 root\n");
        isp.enter("tc qdisc del dev eth0 root\n");
        isp.enter("tc qdisc del dev eth1 root\n");
        server.enter("tc qdisc del dev eth1 root\n");
    }
    }
    }
    }
    gateway.close();
    isp.close();
    server.close();

    // Exit
    return 0;
}

