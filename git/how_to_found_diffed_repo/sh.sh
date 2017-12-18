#!/bin/bash

for i in `ls -d */`; do
	cd $i
	VAL=`git diff`
	if [ "$VAL" != "" ]; then
		echo $i
	fi
	cd ..
done
