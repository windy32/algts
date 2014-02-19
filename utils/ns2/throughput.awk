# Usage: awk -f throughput.awk tracefile bandwidth(mbps) interval(s)
# Example: awk -f throughput.awk link.tr 8 1.0
BEGIN {
    # Statistics
    bytes_sent = 0
    time = 0
    
    # Global statistics
    sum_bytes_sent = 0
    sum_intervals = 0
    
    # Command-line arguments
    bandwidth = ARGV[2]
    interval = ARGV[3]
    ARGC = 2
}
{
    if ($2 > time + interval)
    {
        time = time + interval
        sum_bytes_sent += bytes_sent
        sum_intervals += 1
        bytes_sent = 0
    }
    
    if ($1 == "r")
    {
        bytes_sent += $6
    }
}
END {
    printf("%.1f (%.2f)\n", \
        sum_bytes_sent / 1000 / (sum_intervals * interval), \
        sum_bytes_sent / 1000 / (sum_intervals * interval) / (bandwidth * 128.0));
}

