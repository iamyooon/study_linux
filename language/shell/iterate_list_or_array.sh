#!/bin/bash

#array=( one two three )
array=(
hello
bye
good
)

for i in "${array[@]}"; do
	echo $i
done
