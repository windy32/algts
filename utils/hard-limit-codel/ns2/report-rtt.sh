#!/bin/bash

# Generate a report as shown below, so that it's convenient to paste it into Excel
# The first block 1 Mbps, 2nd block 4 Mbps, 3rd block 16 Mbps, and the last is 64 Mbps
# Each row represnts a different RTT

# 125	125	125			500	500	500			1992	1990	1997			7815	7797	7978
# 121	125	125			473	473	499			1842	1838	1993			7206	7184	7964
# 117	121	123			428	436	498			1700	1696	1988			6692	6666	7934
# 115	115	122			416	422	492			1622	1618	1972			6424	6401	7873
# 108	108	117			403	403	480			1588	1584	1932			6258	6239	7708
# 107	107	115			394	394	469			1558	1557	1888			6223	6192	7510
# 102	101	110			386	376	436			1502	1377	1575			5671	5605	3902
# 99	98	105			375	354	406			1436	1208	1357			4638	4482	2329
# 100	98	102			365	342	384			1309	1035	1123			3406	3280	1578
# 97	95	99			354	322	361			1217	797	793			2670	1161	1162
# 92	89	93			318	280	312			767	450	448			1839	730	726
# 88	84	88			279	236	257			540	292	289			1383	517	514

# Usage: ./report.sh ftps prefixes
# Example: ./report.sh 1 "codel hlc fifo"

if [[ $# != 2 ]]; then
    echo "Error: no file specified!"
    exit 0
fi

ftp=$1
prefixes=$2

for rtt in 10 20 40 60 80 100 150 200 250 300 400 500
do
    block_1m=""
    block_4m=""
    block_16m=""
    block_64m=""

    for queue in $prefixes
    do
        tp_1m=`awk -f print-cell.awk "$queue-1m-rtt$rtt.log" 2 $ftp 6`
        tp_4m=`awk -f print-cell.awk "$queue-4m-rtt$rtt.log" 2 $ftp 6`
        tp_16m=`awk -f print-cell.awk "$queue-16m-rtt$rtt.log" 2 $ftp 6`
        tp_64m=`awk -f print-cell.awk "$queue-64m-rtt$rtt.log" 2 $ftp 6`
        
        if [[ $block_1m == "" ]]; then
            block_1m="$tp_1m"
            block_4m="$tp_4m"
            block_16m="$tp_16m"
            block_64m="$tp_64m"
        else
            block_1m="$block_1m\t$tp_1m"
            block_4m="$block_4m\t$tp_4m"
            block_16m="$block_16m\t$tp_16m"
            block_64m="$block_64m\t$tp_64m"
        fi
    done
    
    echo -e "$block_1m\t\t\t$block_4m\t\t\t$block_16m\t\t\t$block_64m"
done

