#!/bin/bash

# The best buffer size (in packets) for different bandwidth
qlen=(2 3 3 3 3 3 4 4 4 4 4 6 6 6 8 8 10 12 14 16)
qlen3x=(6 9 9 9 9 9 12 12 12 12 12 18 18 18 24 24 30 36 42 48)

# The best percentage of bandwidth used
pct=(0.68 0.75 0.81 0.84 0.85 0.86 0.87 0.89 0.90 0.90 0.91 0.91 0.92 0.92 0.93 0.94 0.94 0.95 0.95 0.96)

for rate in 10 20 40 80 # PackMime web traffic intensity
do
    filename="web-$rate.log"
    i=0

    echo -e "bw\tnum\tpct\tqlen\tmaxq\tavgq\tmaxd\tavgd\trate" > $filename
    echo "---------------------------------------------------------------------------" >> $filename

    for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100 # 10 Loops
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            ns shaping.tcl $num 0 "$bw"Mb 500 ${pct[$i]} ${qlen[$i]}
            echo -ne "$bw\t$num\t${pct[$i]}\t${qlen[$i]}\t" >> $filename
            awk -f queue.awk link-isp.tr $bw 1.0 brief >> $filename
            awk -f throughput.awk link-local.tr $bw 1.0 >> $filename
        done
        i=`expr $i + 1`
    done
done

