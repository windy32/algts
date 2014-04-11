#!/bin/bash

# test-dynamic.sh
if [[ $# == 1 && $1 == "clean" ]]; then
    rm -f dynamic-fifo.log
    rm -f dynamic-codel.log
    rm -f dynamic-codels.log
    rm -f dynamic-hlc.log
    rm -f link.tr
    exit 0
fi

# 417 packets * 1.5 KB = 0.625 MB = 5 Mbit (50ms queue)
ns hl-codel.tcl 4 0 5 100Mb 5 40 1500 fifo 417 dynamic > /dev/null
awk -f queue.awk link.tr 100 0.1 > dynamic-fifo.log
echo -n "."

ns hl-codel.tcl 4 0 5 100Mb 5 40 1500 codel 1000 dynamic > /dev/null
awk -f queue.awk link.tr 100 0.1 > dynamic-codel.log
echo -n "."

# CoDel with a 50ms "small" buffer
ns hl-codel.tcl 4 0 5 100Mb 5 40 1500 codel 417 dynamic > /dev/null
awk -f queue.awk link.tr 100 0.1 > dynamic-codels.log
echo -n "."

ns hl-codel.tcl 4 0 5 100Mb 5 40 1500 hl-codel 50 dynamic > /dev/null
awk -f queue.awk link.tr 100 0.1 > dynamic-hlc.log
echo "."

