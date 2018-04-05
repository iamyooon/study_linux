#!/bin/bash

input_file="cscope.files"
output_file="cscope.out"

cleanup()
{
	rm -rf $@
}

show_menu()
{
	echo "1. make cscope db(all)"
	echo "2. add files to cscope.files"
	echo "3. update cscope db"
	echo -n "select:"
}

mk_cscope_files()
{
	echo "argment cnt=$# list=[$@]"
	op_mode="$1"
	shift 1

	if [ "$op_mode" == "new" ] ; then
		find $@ \( -name '*.c' \
			-o -name '*.cpp' \
			-o -name '*.cc' \
			-o -name '*.h' \
			-o -name '*.s' \
			-o -name '*.S' \) -print > cscope.files
	elif [ "$op_mode" == "update" ]; then
		find $@ \( -name '*.c' \
			-o -name '*.cpp' \
			-o -name '*.cc' \
			-o -name '*.h' \
			-o -name '*.s' \
			-o -name '*.S' \) -print >> cscope.files
	else
		echo "Invalid mode $op_mode"
	fi
}

mk_cscope_out()
{
	cscope -i $1
}

read_user_select()
{
	request="$1"
	arg="$2"

	if [ -z "$request" ] || [ "$request" = "1" ] ; then
		echo "make cscope db(all)"
		cleanup $input_file
		if [ -z "$arg" ] ; then
			mk_cscope_files "."
		else
			shift 2
			mk_cscope_files "new" $@
		fi
		mk_cscope_out $input_file
		exit 0
	elif [ "$request" == "2" ]; then
		echo "add files to cscope.files"
		shift 1
		mk_cscope_files "update" $@
		exit 0
	elif [ "$request" == "3" ]; then
		echo "3. update cscope db"
		mk_cscope_out $input_file
		exit 0
	else
		echo "Invalid input $input"
		exit -1
	fi
}

read_user_select $@
