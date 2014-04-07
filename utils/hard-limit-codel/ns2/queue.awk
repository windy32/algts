# Usage: awk -f queue.awk tracefile bandwidth(mbps) interval(s) [brief | debug]
# Example: awk -f queue.awk link.tr 8 1.0
# Example: awk -f queue.awk link.tr 8 1.0 brief
# Example: awk -f queue.awk link.tr 8 1.0 debug
BEGIN {
    # Statistics
    queue_length = 0
    bytes_sent = 0
    drops = 0
    time = 0
    
    # Global statistics
    max_queue_length = 0
    sum_queue_length = 0
    sum_bytes_sent = 0
    sum_intervals = 0
    
    # Command-line arguments
    bandwidth = ARGV[2]
    interval = ARGV[3]
    if (ARGC == 4)
    {
        brief = 0
        debug = 0
    }
    else if (ARGC == 5 && ARGV[4] == "brief")
    {
        brief = 1
        debug = 0
    }
    else if (ARGC == 5 && ARGV[4] == "debug")
    {
        brief = 0
        debug = 1
    }
    ARGC = 2
    
    if (!brief) # Print title
    {
        print "====================================="
        print "Time    Queue length    Rate    Drops"
        print "-------------------------------------"
    }
}
{
    if ($2 > time + interval)
    {
        time = time + interval
        if (!brief)
        {
            printf("%.1f\t%.0f\t\t%.1f\t%d\n", time, queue_length, bytes_sent / 1000.0 / interval, drops);
        }
        
        sum_queue_length += queue_length
        sum_bytes_sent += bytes_sent
        sum_intervals += 1
        
        drops = 0
        bytes_sent = 0
    }
    
    if ($1 == "+")
    {
        enque_time[$12] = $2
    }
    else if ($1 == "-")
    {
        deque_time[$12] = $2
        queue_length = 1000.0 * (deque_time[$12] - enque_time[$12])
        if (debug)
        {
            printf("Packet %d - %.0lfms\n", $12, queue_length);
        }
    }
    else if ($1 == "r")
    {
        bytes_sent += $6
    }
    else if ($1 == "d")
    {
        drops += 1
    }
    
    if (queue_length > max_queue_length)
        max_queue_length = queue_length
}
END {
    if (!brief)
    {
        print "-------------------------------------"
        printf("Max queue length: %.0f ms\n", max_queue_length);
        printf("Average queue length: %.0f ms\n", sum_queue_length / sum_intervals);
        printf("Average throughput: %.1f KB/s\n", sum_bytes_sent / 1000 / (sum_intervals * interval));
        print "-------------------------------------"
    }
    else # In brief mode, the throughput of the isp link is not displayed, 
         # as we just want the "effective" throughput - the local users' traffic rate
    {
        printf("%.0f\t%.0f\t%.0f (%.2f)\n", \
            max_queue_length, \
            sum_queue_length / sum_intervals, \
            sum_bytes_sent / 1000 / (sum_intervals * interval), \
            sum_bytes_sent / 1000 / (sum_intervals * interval) / (bandwidth * 125.0));
    }
}

