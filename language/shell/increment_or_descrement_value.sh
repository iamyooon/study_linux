#!/bin/bash

# ref
# https://askubuntu.com/questions/385528/how-to-increment-a-variable-in-bash

VALUE=0

let "VALUE=VALUE+1"
echo $VALUE
let "VALUE+=1"
echo $VALUE
let "VALUE++"
echo $VALUE
VALUE=$((VALUE+1))
echo $VALUE
