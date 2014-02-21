BEGIN {
    packets[0] = -1
    packets[1] = 4
    packets[2] = 16
    packets[3] = 64
    packets[4] = 256
    packets[5] = 1024
    i = 0
}
{
    if ($1 == "Bandwidth" && $5 == "threads" && $8 == "ceil") {
        bandwidth = $3
        threads = substr($7, 1, length($7) - 1)
        ceil = substr($10, 1, length($10) - 1)
        
        if (threads == 2) {
            if (i == 0) {
                print "default"
            }
            else
            {
                
                printf("pfifo limit %d\n", packets[i])
            }
            i += 1
        }
        printf("    Threads = %d", threads);
    }
    
    if ($1 == 1 && $2 == "|") {
        min = 0
        max = 0
        avg = 0
    }
    
    min += $3 # TODO: Remove 2000ms+ samples
    max += $5
    avg += $7
    
    arr_max[$1] = $5
    arr_valid[$1] = 1
    
    if ($1 == 50 && $2 == "|") 
    {
        remaining_sum = max
        remaining_num = 50
        
        while (1)
        {
            # Find the max valid element
            max_value = 0
            max_index = 0
            for (j = 1; j <= 50; j++)
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
        printf(" min = %d, max = %d, avg = %d\n", min / 50, remaining_sum / remaining_num, avg / 50)
    }
}
