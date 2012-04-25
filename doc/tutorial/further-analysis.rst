Further Analysis
----------------

As mentioned in the Tracing section, the whole point of running an algts test is to generate output
for study.

You have two basic strategies to work with in algts: using the text trace output generated in test
scripts, or using the gui program.

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

Using algts GUI for Further Analysis
************************************

As mentioned above, the algts gui provides a convenient interface for setting up scenarios, 
executing tests and further analysis.

**To be continued...**

