#!/bin/bash

bandwidth=(1 2 3 4 6 8 10 12 16 20 24 30 40 50 60 80 100) # Mbps
burst_estimate=(5000 5000 5000 7000 7000 7000 7000 7000 7000 7000 7000 7000 7000 7000 8000 10000 12000) # Bytes

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 # 17 Loops
do
    bw=${bandwidth[$i]}
    est=${burst_estimate[$i]}
    
    for j in 60 65 70 75 80 85 90 95 100 105 110 115 120 125 130 135 140 # 17 Loops
    do
        # Calculate burst
        burst=`expr $est \* $j / 100`
        if [[ $burst -lt 4608 ]] ; then
            burst=4608
        fi
        
        # Configure tbf (note: in virtual machines, mtu should be 100000)
        tc qdisc replace dev eth0 root handle 1: tbf burst "$burst"b latency 500ms rate "$bw"mbit mtu 100000
        
        # Print title & progress
        echo -e "\nBandwidth = $bw Mbps, $j%, burst = $burst bytes" >> "tbf-burst.log"
        echo -n "."
        
        # Run
        iperf -c 10.0.0.2 -t 30 -i 1 >> "tbf-burst.log"

        ping 127.0.0.1 -c 2 > /dev/null 
    done
    echo
done

