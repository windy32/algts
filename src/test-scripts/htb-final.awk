BEGIN {
    print "Optimized setting"
    print "========================================================="
    print "Bandwidth | Threads | Ceil | Avg Delay | Max Delay | Rate"
    print "---------------------------------------------------------"
}
{
    if ($1 == "Bandwidth" && $5 == "threads" && $8 == "ceil") {
        bandwidth = $3
        threads = substr($7, 1, length($7) - 1)
        ceil = substr($10, 1, length($10) - 1)
        printf("%9d   %7d   %3d%%   ", bandwidth, threads, ceil);
    }
    
    if ($1 == 1 && $2 == "|") {
        max = 0
        avg = 0
        rate = 0
    }

    if ($1 >= 1 && $1 <= 32 && $2 == "|") {
        max += $5
        avg += $7
        rate += $9

        arr_max[$1] = $5
        arr_valid[$1] = 1
    }

    if ($1 == 32 && $2 == "|") 
    {
        remaining_sum = max
        remaining_num = 32
        
        while (1)
        {
            # Find the max valid element
            max_value = 0
            max_index = 0
            for (j = 1; j <= 32; j++)
            {
                if (arr_valid[j] == 1 && arr_max[j] > max_value)
                {
                    max_value = arr_max[j]
                    max_index = j
                }
            }
                        
            # Remove the bad samples
            if (max_value > (remaining_sum - max_value) / (remaining_num - 1) * 2)
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
        printf("%9d   %9d   %4d\n", avg / 32, remaining_sum / remaining_num, rate / 32);
    }
    
    if (bandwidth == 100000 && threads == 40 && ceil == 96)
    {
        if ($1 == 32 && $2 == "|") 
        {
            print "========================================================="
            print ""
            print "Empirical setting"
            print "========================================================="
            print "Bandwidth | Threads | Ceil | Avg Delay | Max Delay | Rate"
            print "---------------------------------------------------------"
        }
    }
}

END {
    print "========================================================="
}
