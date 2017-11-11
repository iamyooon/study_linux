#!/bin/bash

array1=( one two three )
array2=(
hello
bye
good
)

for i in "${array1[@]}"; do
	echo $i
done

for i in "${array2[@]}"; do
	echo $i
done
