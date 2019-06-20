#!/bin/bash

show_usage()
{
	echo "Usage :"
}

show_param()
{
	echo "param list - $@"
	echo "param nr - $#"
	if [ "$#" -lt 3 ]; then
		echo "given parameter is less than 3"
		show_usage
		exit 1
	fi
	START_NR="$1"
	END_NR="$2"
	FILE_INPUT="$3"
}

get_txt_from_file()
{
	echo "$START_NR, $END_NR, $FILE_INPUT"
	sed -n "$START_NR,$END_NR p" $FILE_INPUT
}

show_param $@
get_txt_from_file
