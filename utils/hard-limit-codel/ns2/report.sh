#!/bin/bash

# Generate a report as shown below, so that it's convenient to paste it into Excel
# The first block is average delay, the second is maximum delay, and the last is throughput
# Each row represnts a different level of congestion (ftp flows)

# 24	27	31			123	39	39			125	125	125
# 21	30	35			183	48	46			123	125	125
# 22	23	39			338	49	47			124	124	125
# 31	22	38			494	50	48			124	124	125
# 39	25	39			775	50	48			125	124	125
# 56	24	39			1335	50	48			125	124	125
# 93	22	39			2248	50	48			125	124	125
# 183	24	39			3686	50	48			125	124	125

# Usage: ./report.sh file1 [file2...]
# Example: ./report.sh {codel,hlc,fifo}-1m.log

if [[ $# == 0 ]]; then
    echo "Error: no file specified!"
    exit 0
fi

for ftp in 1 2 4 8 16 32 64 128
do
    block_avgd=""
    block_maxd=""
    block_throughput=""

    for filename in $*
    do
        avgd=`awk -f print-cell.awk $filename 2 $ftp 5`
        maxd=`awk -f print-cell.awk $filename 2 $ftp 4`
        throughput=`awk -f print-cell.awk $filename 2 $ftp 6`
        
        if [[ $block_avgd == "" ]]; then
            block_avgd="$avgd"
            block_maxd="$maxd"
            block_throughput="$throughput"
        else
            block_avgd="$block_avgd\t$avgd"
            block_maxd="$block_maxd\t$maxd"
            block_throughput="$block_throughput\t$throughput"
        fi
    done
    
    echo -e "$block_avgd\t\t\t$block_maxd\t\t\t$block_throughput"
done

