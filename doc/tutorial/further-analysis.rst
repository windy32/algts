Further Analysis
----------------

As mentioned in the Tracing section, the whole point of running an algts test is to generate output
for study.

You have two basic strategies to work with in algts: using the text trace output generated in test
scripts, or using the raw trace objects within the script.

The Text Trace
**************

Open the file **sample.trace** in the test-scripts directory, and you'll something like the
following:

::

    UserName="Harry" Type="Bulk Download Task"
    {
      0 0 0 0
      1 18 1000 1000
      2 25 1448 2448
      3 29 1448 3896
      4 57 1448 5344
      5 57 1448 6792
      6 63 1448 8240
      ...
      1404 16877 1448 2096320
      1405 16914 1448 2097768
      1406 16943 232 2098000
    }
    UserName="Harry" Type="Bulk Upload Task"
    {
      0 1 0 0
      1 10 1000 1000
      2 10 1000 2000
      3 10 1000 3000
      ...

The text trace file is organized in tasks, and each task belongs to a specific user. The text block
for each task consists of one line specifying the owner and the type of the task, followed by a list
of events called **trace items**:

::

    UserName="..." Type="..."
    {
      ...
    }

For a bulk download task, the first column of the trace item is an index starting with 0, the second
column represents time of the event in milliseconds, the third is the new bytes received, and the
fourth is total bytes received.

    +--------------+------------------------------------+
    | Field        | Description                        |
    +==============+====================================+
    | Index        | Index of the event starting with 0 |
    +--------------+------------------------------------+
    | Time         | Time of the event in milliseconds  |
    +--------------+------------------------------------+
    | NewBytes     | New bytes received                 |
    +--------------+------------------------------------+
    | TotalBytes   | Total bytes received               |
    +--------------+------------------------------------+
    **Table 1: Definition of a bulk download task's trace items**

The trace item for a bulk upload task is similar to that of a bulk download task, except that it
sends data out.

    +--------------+------------------------------------+
    | Field        | Description                        |
    +==============+====================================+
    | Index        | Index of the event starting with 0 |
    +--------------+------------------------------------+
    | Time         | Time of the event in milliseconds  |
    +--------------+------------------------------------+
    | NewBytes     | New bytes sent                     |
    +--------------+------------------------------------+
    | TotalBytes   | Total bytes sent                   |
    +--------------+------------------------------------+
    **Table 2: Definition of a bulk upload task's trace items**

The trace item for an on/off download task can be regarded as a combination of several bulk download
task items. An on/off download task may consist of several **on** and **off** periods, and the
download session lies within each **on** period. Each time an **on** period is passed, **Index**
starts again with 0.

    +--------------+----------------------------------------------------------+
    | Field        | Description                                              |
    +==============+==========================================================+
    | Period       | Index of the period starting with 0                      |
    +--------------+----------------------------------------------------------+
    | Index        | Index of the event within current period starting with 0 |
    +--------------+----------------------------------------------------------+
    | Time         | Time of the event in milliseconds                        |
    +--------------+----------------------------------------------------------+
    | NewBytes     | New bytes received in current period                     |
    +--------------+----------------------------------------------------------+
    | TotalBytes   | Total bytes received in current period                   |
    +--------------+----------------------------------------------------------+
    **Table 3: Definition of a on/off download task's trace items**

The TCP echo task and asynchronous UDP echo task has a simpler trace item structure, as the basic
idea of these tasks is to send a packet, and calculate the time between the packet and its echo.

    +--------------+------------------------------------+
    | Field        | Description                        |
    +==============+====================================+
    | Index        | Index of the event starting with 0 |
    +--------------+------------------------------------+
    | Time         | Time when the input packet is sent |
    +--------------+------------------------------------+
    | Delay        | The echo delay in milliseconds     |
    +--------------+------------------------------------+
    **Table 4: Definition of a tcp echo task / asynchronous udp echo task**

Note that for an asynchronous UDP echo task, the input packet or echo packet can be lost, in which
case the delay is set to -1.

Now since you're clear how text trace output is organized, pick your favourite programming language,
e.g., awk, perl, python, c/c++, and transform the text trace into your desired format.

.. note::

    Unliky a discrete event simulator like ns-2 / ns-3, the trace file generated by algts is not
    absolutely stable even a whole random variable subsystem and seeding support has been included
    in algts. Therefore it is recommended to run a test with a same seed several times before
    actually starting with analysis.

Using Raw Trace Objects for Further Analysis
********************************************

Though it is possible to execute a test, get text trace files, and then use tools like awk for
further analysis, it is not recommend in algts as it is just not convenient.

In algts scripts, users can get the trace item of a specific task as a **RawTraceItem** object, the
difinition of class **RawTraceItem** is given below:

::

    typedef QMap<QString, QList<qint32> > RawTraceItem;

The key of the map is the field name of the trace item, e.g., for a tcp echo task, the key could be
"Index", "Time", or "Delay", and the value to the key is a list of integers containing the data of
the field.

Different fields of the trace item always share a same length. As a result, code to traverse a raw
trace item of a tcp echo task often looks like:

::

    for (int i = 0; i < traceItem["Index"].size(); i++)
    {
        int index = traceItem["Index"][i];
        int time  = traceItem["Time"][i];
        int delay = traceItem["Delay"][i];
        ...
    }

Now that users can get any trace item conveniently, any analysis is possible. Below is a example of 
a complete script function that executes the test, collects statistics and finally prints formatted
output, when users want to investigate how download may affact queuing delay:

::

    void execTest(ConsoleApplication &app, 
                  int bulkUsers, int nThreads, int *threads, // Test options
                  bool printDetail, bool printSummary) // Output options
    {
        QVector<int> mins, maxs, avgs;
        
        for (int index = 0; index < nThreads; index++) // n threads
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
                sprintf(username, "Bulk Download User %d", i + 1);
                s.addUser(username);

                for (int j = 0; j < n; j++)
                {
                    s.addTask(username, new BulkDownloadTask(80));
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

When a simple fifo queue is applied at the router, and there are three bulk download users plus one
to fifteen download threads, the output generated is as shown below:

::

    Pass 1 / 15, 1 Threads
    ------------------------------------------
    221 Samples
    Min delay: 2ms, Max delay: 211ms, Avg delay: 35ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
     110  51  17  20  11   2   4   3   2   0   1   0   0   0   0   0   0   0   0   0   0

    Pass 2 / 15, 2 Threads
    ------------------------------------------
    144 Samples
    Min delay: 2ms, Max delay: 252ms, Avg delay: 60ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
      22  40  23  22  12  13   3   2   2   1   1   2   1   0   0   0   0   0   0   0   0

    Pass 3 / 15, 3 Threads
    ------------------------------------------
    101 Samples
    Min delay: 1ms, Max delay: 273ms, Avg delay: 89ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       6  13  11  23  10  11   9   7   5   1   2   1   1   1   0   0   0   0   0   0   0

    Pass 4 / 15, 4 Threads
    ------------------------------------------
    100 Samples
    Min delay: 2ms, Max delay: 289ms, Avg delay: 89ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       3   8   9  33  19   8   8   5   0   2   2   1   0   1   1   0   0   0   0   0   0

    Pass 5 / 15, 5 Threads
    ------------------------------------------
    75 Samples
    Min delay: 2ms, Max delay: 341ms, Avg delay: 124ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       2   3   8   9  11   9   6   8   4   1   4   4   3   0   0   1   1   1   0   0   0

    Pass 6 / 15, 6 Threads
    ------------------------------------------
    72 Samples
    Min delay: 32ms, Max delay: 293ms, Avg delay: 129ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   2   3   6  13  11  10   9   6   7   0   2   1   1   1   0   0   0   0   0   0

    Pass 7 / 15, 7 Threads
    ------------------------------------------
    63 Samples
    Min delay: 69ms, Max delay: 346ms, Avg delay: 150ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   3   5  14  10  11   8   3   3   1   0   2   1   0   1   1   0   0   0

    Pass 8 / 15, 8 Threads
    ------------------------------------------
    53 Samples
    Min delay: 67ms, Max delay: 389ms, Avg delay: 178ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   2   2   3   8  11   9   2   3   4   2   2   1   0   1   2   0   1   0

    Pass 9 / 15, 9 Threads
    ------------------------------------------
    52 Samples
    Min delay: 44ms, Max delay: 327ms, Avg delay: 183ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   1   2   2   5   6   8   5   2   6   4   1   2   5   1   2   0   0   0   0

    Pass 10 / 15, 10 Threads
    ------------------------------------------
    41 Samples
    Min delay: 74ms, Max delay: 713ms, Avg delay: 239ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   1   0   0   2   4  12   2   4   2   3   2   2   0   0   1   2   0   4

    Pass 11 / 15, 11 Threads
    ------------------------------------------
    43 Samples
    Min delay: 82ms, Max delay: 410ms, Avg delay: 229ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   0   2   2   2   0   3   6   5   7   4   3   2   0   2   1   1   2   1

    Pass 12 / 15, 12 Threads
    ------------------------------------------
    35 Samples
    Min delay: 56ms, Max delay: 548ms, Avg delay: 279ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   1   0   0   0   0   0   3   2   8   4   3   0   5   1   1   0   1   0   6

    Pass 13 / 15, 13 Threads
    ------------------------------------------
    33 Samples
    Min delay: 192ms, Max delay: 516ms, Avg delay: 294ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   0   0   0   0   0   0   2   2   8   5   3   3   2   1   1   0   0   6

    Pass 14 / 15, 14 Threads
    ------------------------------------------
    35 Samples
    Min delay: 86ms, Max delay: 527ms, Avg delay: 280ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   0   1   0   1   1   0   3   0   1   4   5   8   4   2   1   1   0   3

    Pass 15 / 15, 15 Threads
    ------------------------------------------
    31 Samples
    Min delay: 117ms, Max delay: 609ms, Avg delay: 317ms
      20  40  60  80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 400+
       0   0   0   0   0   1   0   0   1   1   0   2   6   5   0   1   6   1   1   0   6

    ======================================================================================
    Threads   |    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
    ----------+---------------------------------------------------------------------------
    Min Delay |    2    2    1    2    2   32   69   67   44   74   82   56  192   86  117
    Max Delay |  211  252  273  289  341  293  346  389  327  713  410  548  516  527  609
    Avg Delay |   35   60   89   89  124  129  150  178  183  239  229  279  294  280  317
    ======================================================================================

