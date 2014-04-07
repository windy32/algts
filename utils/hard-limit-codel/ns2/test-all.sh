#!/bin/bash

# test-all.sh

if [[ $# == 1 && $1 == "clean" ]]; then
    ./test-codel.sh clean
    ./test-hlc.sh clean
    ./test-fifo.sh clean
    ./test-dynamic.sh clean
    exit 0
fi

./test-codel.sh
./test-hlc.sh
./test-fifo.sh
./test-dynamic.sh

