#!/bin/bash

# test-codel.sh

# Global setting (rtt): 10, 20, 40, 60, 80, 100, 150, 200, 250, 300, 400, 500
delay_=(1 2 4 5 5 5 5 5 5 5 5 5)
bdelay_=(3 6 12 20 30 40 65 90 115 140 190 240)

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
    echo -n "                                                            "
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
    if [ $# != 8 ]; then
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
    mode=$8

    if [[ $mode == "fifo" ]]; then
        qlen=`expr $bw \* 1000 \* 50 / 8 / 1500 + 1` # qlen = 50ms
    elif [[ $mode == "codel" ]]; then
        qlen=1000
    else
        qlen="-"
    fi

    echo -e "bw\tftp\tqlen\tmaxd\tavgd\trate" > $filename
    echo "--------------------------------------------------" >> $filename

    pass=0
    for ftp in 1 2 4 8 16 32 64 128 # 8 Loops
    do
        if [[ $mode == "fifo" ]]; then
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 fifo $qlen > /dev/null
        elif [[ $mode == "codel" ]]; then
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 codel 1000 > /dev/null
        else
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 hl-codel 50 > /dev/null
        fi
        echo -ne "$bw\t$ftp\t$qlen\t" >> $filename
        awk -f queue.awk link.tr $bw 1.0 brief >> $filename
        draw_progress_bar 8 $((++pass)) "$description"
    done
}

if [[ $# == 1 && $1 == "clean" ]]; then
    for bw in 1 4 16 64
    do
        for index in 0 1 2 3 4 5 6 7 8 9 10 11
        do
            d=${delay_[$index]}
            bd=${bdelay_[$index]}
            rtt=`expr $d \* 4 + $bd \* 2`
            rm -f "fifo-$bw""m-rtt$rtt.log"
            rm -f "codel-$bw""m-rtt$rtt.log"
            rm -f "hlc-$bw""m-rtt$rtt.log"
        done
    done
    rm -f link.tr
    exit 0
fi

# Save cursor position
tput sc

# total = 4 * 12 * 3 = 144
total=144
progress=1

for bw in 1 4 16 64
do
    for index in 0 1 2 3 4 5 6 7 8 9 10 11
    do
        d=${delay_[$index]}
        bd=${bdelay_[$index]}
        rtt=`expr $d \* 4 + $bd \* 2`
        
        execute_test "[$progress/$total] Running test for $bw Mbps, RTT $rtt""ms, fifo " "fifo-$bw""m-rtt$rtt.log" $bw 0 0 $d $bd fifo
        ((progress++))
        execute_test "[$progress/$total] Running test for $bw Mbps, RTT $rtt""ms, codel" "codel-$bw""m-rtt$rtt.log" $bw 0 0 $d $bd codel
        ((progress++))
        execute_test "[$progress/$total] Running test for $bw Mbps, RTT $rtt""ms, hlc  " "hlc-$bw""m-rtt$rtt.log" $bw 0 0 $d $bd hlc
        ((progress++))
    done
done

echo ""
