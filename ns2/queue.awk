# Usage: awk -f queue.awk tracefile bandwidth(mbps) interval(s) [brief]
# Example: awk -f queue.awk link.tr 8 1.0
# Example: awk -f queue.awk link.tr 8 1.0 brief
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
    }
    else if (ARGC == 5)
    {
        brief = 1
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
            printf("%.1f\t%d\t\t%.1f\t%d\n", time, queue_length, bytes_sent / 1000.0, drops);
        }
        
        sum_queue_length += queue_length
        sum_bytes_sent += bytes_sent
        sum_intervals += 1
        
        drops = 0
        bytes_sent = 0
    }
    
    if ($1 == "+")
    {
        queue_length += $6
    }
    else if ($1 == "-")
    {
        queue_length -= $6
    }
    else if ($1 == "r")
    {
        bytes_sent += $6
    }
    else if ($1 == "d")
    {
        queue_length -= $6
        drops += 1
    }
    
    if (queue_length > max_queue_length)
        max_queue_length = queue_length
}
END {
    if (!brief)
    {
        print "-------------------------------------"
        printf("Max queue length: %.1f KB\n", max_queue_length / 1000.0);
        printf("Average queue length: %.1f KB\n", sum_queue_length / sum_intervals / 1000.0);
        printf("Average throughput: %.1f KB/s\n", sum_bytes_sent / 1000 / (sum_intervals * interval));
        print "-------------------------------------"
    }
    else # In brief mode, the throughput of the isp link is not displayed, 
         # as we just want the "effective" throughput - the local users' traffic rate
    {
        printf("%.1f\t%.1f\t%d\t%d\t", \
            max_queue_length / 1000.0, \
            sum_queue_length / sum_intervals / 1000.0, \
            (max_queue_length / 1000.0) / (bandwidth * 128.0) * 1000.0, \
            (sum_queue_length / sum_intervals / 1000.0) / (bandwidth * 128.0) * 1000.0);
    }
}

