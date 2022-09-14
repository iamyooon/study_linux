#!/bin/bash

if [ "$1" == "" ]; then
	echo "argue[1] is empty"
else
	echo "argue[1] = $1"
fi

sz=1024

if [ "$sz" -gt "1024" ]; then
	echo "$sz is greater than 1024";
elif [ "$sz" -ge "1024" ]; then
	echo "$sz is greater than 1024 or equal";
else
	echo "$sz is smaller than 1024";
fi
