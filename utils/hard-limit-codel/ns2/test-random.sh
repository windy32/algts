#!/bin/bash

# test-codel.sh

# Global setting (rtt): 10, 30, 50, 100, 150, 200, 300, 500
delay_=(1 2 4 5 5 5 5 5)
bdelay_=(3 11 17 40 65 90 140 240)

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
    if [ $# != 5 ]; then
        echo "Illegal arguments"
        exit 0
    fi
    description=$1
    filename=$2
    delay=$3
    bdelay=$4
    mode=$5

	# Calculate queue length
    if [[ $mode == "fifo" ]]; then
        qlen=`expr $bw \* 1000 \* 50 / 8 / 1500 + 1` # qlen = 50ms
    elif [[ $mode == "codel" ]]; then
        qlen=1000
    else
        qlen="-"
    fi
    

    echo -e "bw\tftp\trftp\tweb\tqlen\tmaxd\tavgd\trate" > $filename
    echo "--------------------------------------------------------------------" >> $filename

    for ((pass = 0; pass < 120; pass++))
    do
		# Generate random parameters
		((ftp=$RANDOM % 64 + 1)) # 1 to 64
		((bw=$RANDOM % 100 + 1)) # 1 to 100
		((rev=$RANDOM % 5))      # 0 to 4
		((web=$RANDOM % 81))     # 0 to 80

        progress=`expr $pass / 3 + 1`
        draw_progress_bar 40 $progress "$description"

        if [[ $mode == "fifo" ]]; then
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 fifo $qlen > /dev/null
        elif [[ $mode == "codel" ]]; then
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 codel 1000 > /dev/null
        else
            ns hl-codel.tcl $ftp $rev $web "$bw"Mb $delay $bdelay 1500 hl-codel 50 > /dev/null
        fi
        echo -ne "$bw\t$ftp\t$rev\t$web\t$qlen\t" >> $filename
        awk -f queue.awk link.tr $bw 1.0 brief >> $filename
        
    done
}

if [[ $# == 1 && $1 == "clean" ]]; then
    for index in 0 1 2 3 4 5 6 7
    do
        d=${delay_[$index]}
        bd=${bdelay_[$index]}
        rtt=`expr $d \* 4 + $bd \* 2`

        rm -f "rnd-hlc-rtt$rtt.log"
        rm -f "rnd-fifo-rtt$rtt.log"
        rm -f "rnd-codel-rtt$rtt.log"
    done
    rm -f link.tr
    exit 0
fi

# Save cursor position
tput sc

# Fix the seed, so that we'll get the same result from the script
RANDOM=12345

for index in 0 1 2 3 4 5 6 7
do
    d=${delay_[$index]}
    bd=${bdelay_[$index]}
    rtt=`expr $d \* 4 + $bd \* 2`
    pgs=`expr $index + 1`

    execute_test "[$pgs/8] RTT $rtt""ms, hlc  " "rnd-hlc-rtt$rtt.log" $d $bd hlc
    execute_test "[$pgs/8] RTT $rtt""ms, fifo " "rnd-fifo-rtt$rtt.log" $d $bd fifo
    execute_test "[$pgs/8] RTT $rtt""ms, codel" "rnd-codel-rtt$rtt.log" $d $bd codel
done

echo ""
