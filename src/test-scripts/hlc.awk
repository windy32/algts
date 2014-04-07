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
        
        arr_max[$1] = $5
        arr_avg[$1] = $7
        arr_valid[$1] = 1
    }

    if ($1 == 3 && $2 == "|") 
    {
        remaining_sum = max
        remaining_num = 3
        
        while (1)
        {
            # Find the max valid element
            max_value = 0
            max_index = 0
            for (j = 1; j <= 3; j++)
            {
                if (arr_valid[j] == 1 && arr_max[j] > max_value)
                {
                    max_value = arr_max[j]
                    max_index = j
                }
            }
                        
            # Remove the bad samples
            if (max_value > (remaining_sum - max_value) / (remaining_num - 1) * 3)
            {
                arr_valid[max_index] = 0
                remaining_sum -= max_value
                remaining_num -= 1
                # printf(" <removing %d> ", max_value)
            }
            else
            {
                break
            }
        }
        printf("%9d   %9d   %4d\n", avg / 3, remaining_sum / remaining_num, rate / 3);
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
                print "========================================================"
                print "RTT | Bandwidth | Threads | Avg Delay | Max Delay | Rate"
                print "--------------------------------------------------------"
            }
        }
    }
}

