#!/bin/bash

FILE1=./FILE1
FILE2=./FILE2

if [ "$1" != "" ]; then
	FILE1="$1"
	echo "use $FILE1 as file1"
fi

if [ "$2" != "" ]; then
	FILE2="$2"
	echo "use $FILE2 as file1"
fi
echo "print only unique lines of FILE1"
comm -23 $FILE1 $FILE2
echo "print only unique lines of FILE2"
comm -13 $FILE1 $FILE2
echo "print only unique lines of FILE1 and FILE2"
comm -3 $FILE1 $FILE2
echo "print lines that appear in both files"
comm -12 $FILE1 $FILE2

