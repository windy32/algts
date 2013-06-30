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
 * \file rx-delay.cpp
 * \ingroup Scripts
 * \brief The sample client application script
 *
 * Usage:
 * \code
 * rx-delay <local-address-range> <daemon-address> <daemon-port>
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
    emulator.setParam("TxRate", "500kbps");
    emulator.setParam("RxRate", "2000kbps");
    emulator.commit();

    QVector<int> mins;
    QVector<int> maxs;
    QVector<int> avgs;

    for (int n = 1; n <= 15; n++)
    {
        printf("\nRx Delay Test Pass %d / %d\n", n, 15);
        printf("---------------------------------------------\n");

        // Setup scenario
        Scenario s(12345, 20); // seed & length

        s.addUser("Realtime User");
        s.addTask("Realtime User", new TcpEchoTask(23));
        s.task()->setAttribute("InputSize", "Uniform 100, 100");
        s.task()->setAttribute("EchoSize", "Uniform 100, 100");
        s.task()->setAttribute("Interval", "Uniform 10, 10");

        s.addUser("Bulk Download User");
        for (int i = 0; i < n; i++)
        {
            s.addTask("Bulk Download User", new BulkDownloadTask(80));
            s.task()->setAttribute("MaxBytes", "INFINITE");
            s.task()->setAttribute("MaxRate", "INFINITE");
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

    // Summary output
    printf("\nSummary\n");
    printf("=========================================================================\n");
    printf("             ");
    for (int i = 0; i < 15; i++)
    {
        printf("  %02d", i + 1);
    }
    printf("\n-------------------------------------------------------------------------");

    printf("\nMin delay(ms)");
    for (int i = 0; i < 15; i++)
    {
        printf("%4d", mins[i]);
    }
    
    printf("\nMax delay(ms)");
    for (int i = 0; i < 15; i++)
    {
        printf("%4d", maxs[i]);
    }

    printf("\nAvg delay(ms)");
    for (int i = 0; i < 15; i++)
    {
        printf("%4d", avgs[i]);
    }
    printf("\n=========================================================================\n");

    // Exit
    return 0;
}

