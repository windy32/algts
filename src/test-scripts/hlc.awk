BEGIN {
    print "fifo"
    print "========================================================"
    print "RTT | Bandwidth | Threads | Avg Delay | Max Delay | Rate"
    print "--------------------------------------------------------"
    algorithm = 1
}
{
    if ($1 == "RTT" && $5 == "Bandwidth" && $9 == "threads") {
        rtt = $3
        bandwidth = $7
        threads = $11
        ceil = substr($10, 1, length($10) - 1)
        printf("%3d   %9d   %7d   ", rtt, bandwidth, threads);
    }
    
    if ($1 == 1 && $2 == "|") {
        max = 0
        avg = 0
        rate = 0
    }

    if ($1 >= 1 && $1 <= 3 && $2 == "|") {
        max += $5
        avg += $7
        rate += $9
    }

    if ($1 == 3 && $2 == "|") 
    {
        printf("%9d   %9d   %4d\n", avg / 3, max / 3, rate / 3);
    }

    if (rtt == 500 && bandwidth == 64000 && threads == 64)
    {
        if ($1 == 3 && $2 == "|") 
        {
            print "========================================================="
            print ""
            algorithm += 1
            if (algorithm == 2)
                print "codel"
            else if (algorithm == 3)
                print "hlc"
            if (algorithm == 2 || algorithm == 3) {
                print "========================================================="
                print "Bandwidth | Threads | Ceil | Avg Delay | Max Delay | Rate"
                print "---------------------------------------------------------"
            }
        }
    }
}

