#!/bin/bash

# test-codel.sh

draw_progress_bar()
{
    # Example:
    # [>  ] cur = 1, max = 3
    # [-> ] ...
    # [-->] cur = 3, max = 3
    max=$1
    cur=$2
    desc=$3

    # Restore cursor position
    tput rc

    echo -n "["
    for ((i = 0; i < cur - 1; i++))
    do
        echo -n "-"
    done
    echo -n ">"
    for ((i = 0; i < max - cur; i++))
    do
        echo -n " "
    done
    echo -n "] "

    echo -ne "\033[1m\033[31m"
    echo -n $desc
    echo -ne "\033[0m"
}

execute_test()
{
    if [ $# != 7 ]; then
        echo "Illegal arguments"
        exit 0
    fi
    description=$1
    filename=$2
    bw=$3
    rev=$4
    web=$5
    delay=$6
    bdelay=$7
    
    qlen=1000
        
    echo -e "bw\tftp\tqlen\tmaxd\tavgd\trate" > $filename
    echo "--------------------------------------------------" >> $filename

    pass=0
    for ftp in 1 2 4 8 16 32 64 128 # 8 Loops
    do
        ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 codel 1000 > /dev/null
        echo -ne "$bw\t$ftp\t$qlen\t" >> $filename
        awk -f queue.awk link.tr $bw 1.0 brief >> $filename
        draw_progress_bar 8 $((++pass)) "$description"
    done
}

if [[ $# == 1 && $1 == "clean" ]]; then
    for rtt in 10 100 500
    do
        for bw in 1 4 16 64
        do
            rm -f "codel-$bw""m-rtt$rtt.log"
        done
    done
    for rftp in 1 2 4
    do
        for bw in 1 4 16 64
        do
            rm -f "codel-$bw""m-r$rftp.log"
        done
    done
    for web in 10 20 40 80
    do
        for bw in 1 4 16 64
        do
            rm -f "codel-$bw""m-w$web.log"
        done
    done
    rm -f link.tr
    exit 0
fi

# Save cursor position
tput sc

# total = 4 + 4 + 4 + 3 * 4 + 4 * 4 = 40
total=40
progress=1

# Default RTT: 100ms
for bw in 1 4 16 64
do
    execute_test "[$progress/$total] Running test for $bw Mbps" "codel-$bw""m-rtt100.log" $bw 0 0 5 40
    ((progress++))
done

# Small RTT: 10ms
for bw in 1 4 16 64
do
    execute_test "[$progress/$total] Running test for $bw Mbps" "codel-$bw""m-rtt10.log" $bw 0 0 1 3
    ((progress++))
done

# Large RTT: 500ms
for bw in 1 4 16 64
do
    execute_test "[$progress/$total] Running test for $bw Mbps" "codel-$bw""m-rtt500.log" $bw 0 0 5 240
    ((progress++))
done

# Default RTT, with reverse ftp
for rftp in 1 2 4
do
    for bw in 1 4 16 64
    do
        execute_test "[$progress/$total] Running test for $bw Mbps" "codel-$bw""m-r$rftp.log" $bw $rftp 0 5 50
        ((progress++))
    done
done

# Default RTT, with web traffic
for web in 10 20 40 80
do
    for bw in 1 4 16 64
    do
        execute_test "[$progress/$total] Running test for $bw Mbps" "codel-$bw""m-w$web.log" $bw 0 $web 5 50
        ((progress++))
    done
done

echo ""
