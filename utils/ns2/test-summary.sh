#!/bin/bash

low_bandwidth()
{
    for bw in 0.5 1 2 3 # Low bandwidth (4 Loops)
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            for pct in 0.50 0.52 0.54 0.56 0.58 0.60 0.62 0.64 0.66 0.68 \
                       0.70 0.72 0.74 0.76 0.78 0.80 0.82 0.84 0.86 0.88 \
                       0.90 0.92 0.94 0.96 0.98 1.00 # 26 Loops
            do
                for qlen in 2 3 4 5 6 7 8 # 7 Loops
                do
                    ns shaping.tcl $num 0 "$bw"Mb 1500 $pct $qlen
                    echo -ne "$bw\t$num\t$pct\t$qlen\t" >> link-summary.log
                    awk -f queue.awk link-isp.tr $bw 1.0 brief >> link-summary.log
                    awk -f throughput.awk link-local.tr $bw 1.0 >> link-summary.log
                done
            done
        done
    done
}

medium_bandwidth()
{
    for bw in 4 5 6 8 # Medium bandwidth (4 Loops)
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            for pct in 0.80 0.82 0.84 0.86 0.88 0.90 0.92 0.94 0.96 0.98 1.00 # 11 Loops
            do
                for qlen in 2 3 4 5 6 7 8 # 7 Loops
                do
                    ns link.tcl $num 0 "$bw"Mb 1500 $pct $qlen
                    echo -ne "$bw\t$num\t$pct\t$qlen\t" >> link-summary.log
                    awk -f queue.awk link-isp.tr $bw 1.0 brief >> link-summary.log
                    awk -f throughput.awk link-local.tr $bw 1.0 >> link-summary.log
                done
            done
        done
    done
}

high_bandwidth()
{
    for bw in 10 12 14 16 20 24 # High bandwidth (6 Loops)
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            for pct in 0.80 0.82 0.84 0.86 0.88 0.90 0.92 0.94 0.96 0.98 1.00 # 11 Loops
            do
                for qlen in 2 3 4 5 6 7 8 10 12 16 # 10 Loops
                do
                    ns link.tcl $num 0 "$bw"Mb 1500 $pct $qlen
                    echo -ne "$bw\t$num\t$pct\t$qlen\t" >> link-summary.log
                    awk -f queue.awk link-isp.tr $bw 1.0 brief >> link-summary.log
                    awk -f throughput.awk link-local.tr $bw 1.0 >> link-summary.log
                done
            done
        done
    done
}

very_high_bandwidth()
{
    for bw in 30 40 50 60 80 100 # Very high bandwidth (6 Loops)
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            for pct in 0.90 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 1.00 # 11 Loops
            do
                for qlen in 4 6 8 10 12 14 16 18 20 32 64 128 256 # 13 Loops
                do                                      
                    ns link.tcl $num 0 "$bw"Mb 1500 $pct $qlen
                    echo -ne "$bw\t$num\t$pct\t$qlen\t" >> link-summary.log
                    awk -f queue.awk link-isp.tr $bw 1.0 brief >> link-summary.log
                    awk -f throughput.awk link-local.tr $bw 1.0 >> link-summary.log
                done
            done
        done
    done
}

echo -e "bw\tnum\tpct\tqlen\tmaxq\tavgq\tmaxd\tavgd\trate" > link-summary.log
echo "---------------------------------------------------------------------------" >> link-summary.log

if [ $# -eq 0 ]; then
    low_bandwidth
    medium_bandwidth
    high_bandwidth
    very_high_bandwidth
elif [ $# -eq 1 ]; then
    if [ $1 = "low" ]; then
        low_bandwidth
    elif [ $1 = "medium" ]; then
        medium_bandwidth
    elif [ $1 = "high" ]; then
        high_bandwidth
    elif [ $1 = "very-high" ]; then
        very_high_bandwidth
    fi
fi

