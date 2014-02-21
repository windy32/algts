#!/bin/bash

bandwidth=(1 2 3 4 6 8 10 12 16 20 24 30 40 50 60 80 100) # Mbps
burst_estimate=(3080 3200 4000 5000 5000 7000 8000 10000 12000 14000 15000 16000 20000 24000 28000 30000 40000) # Bytes

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 # 17 Loops
do
    bw=${bandwidth[$i]}
    est=${burst_estimate[$i]}
    
    for j in 60 65 70 75 80 85 90 95 100 105 110 115 120 125 130 135 140 # 17 Loops
    do
        # Calculate burst
        burst=`expr $est \* $j / 100`
        if [[ $burst -lt 3080 ]] ; then
            burst=3080
        fi
        
        # Configure tbf (note: in virtual machines, mtu should be 100000)
        sudo tc qdisc replace dev eth0 root handle 1: tbf burst "$burst"b latency 500ms rate "$bw"mbit mtu 100000
        
        # Print title & progress
        echo -e "\nBandwidth = $bw Mbps, $j%, burst = $burst bytes" >> "tbf-burst.log"
        echo -n "."
        
        # Run
        iperf -c 10.0.2.2 -t 50 -i 1 >> "tbf-burst.log"
    done
    echo
done

