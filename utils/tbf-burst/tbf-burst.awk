BEGIN {
    print "======================================="
    print "Bandwidth | Offset | Burst | Throughput"
    print "---------------------------------------"
}
{
    if (NF == 9 && $1 == "Bandwidth" && $6 == "burst")
    {
        if (unit == "kbit")
        {
            printf("%10d\n", value);
        }
        else if (unit == "mbit")
        {
            printf("%10d\n", value * 1000);
        }
    
        bandwidth = $3
        percentage = substr($5, 0, length($5) - 2)
        burst = $8
        
        printf("%9d   %5d%%   %5d   ", bandwidth, percentage, burst);
    }
    
    if ($1 == "[" && $2 == "3]")
    {
        if ($8 == "Kbits/sec")
        {
            unit = "kbit"
            value = $7
        }
        else if ($8 == "Mbits/sec")
        {
            unit = "mbit"
            value = $7
        }
    }
}

END {
    if (unit == "kbit")
    {
        printf("%10d\n", value);
    }
    else if (unit == "mbit")
    {
        printf("%10d\n", value * 1000);
    }
    print "======================================="
}
