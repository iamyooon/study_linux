#!/bin/bash

NR_THREADS=1
if [ "$1" ]; then
	NR_THREADS=$1
fi

for i in `seq 1 $NR_THREADS`; do
	while true; do
		:
	done&
done

trap "kill -- -$$" SIGTERM
wait
