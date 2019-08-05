#!/bin/bash
INPUT=""
OUTPUT=""
DEBUG="0"
OP_MODE="all"
FILELIST="cpu0 cpu1 cpu2 cpu3"
PID=""
#FILELIST="cpu0"

get_only_switch_field()
{
	cat $1 | grep "sched_switch:" > output/$1.only_switch
}

make_it_csv()
{
	cat $1 | cut -c'34-46' | tr -d ' ' > $1.timestamp
	cat $1 | cut -c'62-' | sed 's/prev_comm=//g;s/prev_pid=/,/g;s/prev_prio=/,/g;s/prev_state=/,/g;s/==> next_comm=/,/g;s/next_pid=/,/g;s/next_prio=/,/g' | sed 's/ ,/,/g;s/^ //g' | tr -d '[\[\]\/\ ]' > $1.switchdata
	paste -d',' $1.timestamp $1.switchdata > $1.csv
}

find_all_taskname_percpu()
{
	for filename in $FILELIST; do
		cat output/$filename.only_switch.csv | cut -d',' -f2,3 >> output/$filename.tasklist
	done
}

get_uniq_tasklist()
{
	echo "Merge all taskname"
	for filename in $FILELIST; do
		cat output/$filename.tasklist | sort | uniq >> output/tasklist
	done
}

grep_info_with_name_n_pid()
{
	for filename in $FILELIST; do
		if [ "$DEBUG" == "1" ]; then
			echo "cat output/$filename.only_switch.csv | grep ",$1,$2," >> output/pertask/$1.$2"
		fi
		cat output/$filename.only_switch.csv | grep ",$1,$2," >> output/pertask/$1.$2.tmp
	done
	sort -n output/pertask/$1.$2.tmp > output/pertask/$1.$2
	rm output/pertask/$1.$2.tmp
}

get_cpuusage_for_process()
{
	IFS=',' read -r -a array <<< "$1"
	if [ "$DEBUG" == "1" ]; then
		echo "name=${array[0]} pid=${array[1]}"
	fi
	grep_info_with_name_n_pid "${array[0]}" "${array[1]}"
}

split_event_into_pertask()
{
	rm -rf output/pertask
	mkdir -p output/pertask

	total=`cat $1 | wc -l`
	echo "split event for $total task"

	for taskinfo in `cat $1`; do
		get_cpuusage_for_process $taskinfo
	done

}
get_cpuusage_for_task()
{
	if [ "$DEBUG" == "1" ] && [ -f "output/pertask/elapsed/$1" ]; then
		echo "output/pertask/elpased/$1 is exist, remove it"
		rm output/pertask/elapsed/$1
	fi

	for period in `cat output/pertask/$1 | cut -d',' -f1 | xargs -n2 | tr ' ' ','`; do
		IFS=',' read -r -a array <<< "$period"
		if [ "$DEBUG" == "1" ]; then
			echo $period,start=${array[0]} end=${array[1]}
		fi
		echo `echo "${array[1]} - ${array[0]}" | bc -l` >> output/pertask/elapsed/$1
	done
}

get_cpuusage_for_pertask()
{
	mkdir -p output/pertask/elapsed

	total=20
	echo "split event for $total task"

	echo "Calculate per-task cpuusage for $total task"
	# TBD. check a mismatch between `find output/pertask` and `cat output/tasklist`
	#for filename in `find output/pertask | sort`; do
	COUNTER="1"
	# temporarily, we dont use passed tasklist
	#for filename in `cat $1 | tr ',' '.' `; do

	# ref. http://faq.hostway.co.kr/Linux_ETC/3799
	for filename in `ls -S output/pertask | head -n20`; do
		#get name & pid from filename
		echo "($COUNTER/$total) - $filename"
		get_cpuusage_for_task $filename
		COUNTER=$(expr $COUNTER + 1)
	done
}

fix_data()
{
	filename=`basename $1`
	taskpid=`echo $filename | cut -d'.' -f2`
	taskname=`echo $filename | cut -d'.' -f1`

	# line format
	# timestamp,prev name,prev pid,prev priority,status,next name,next pid,next priority
	IFS=',' read -r -a array <<< `head -n1 $1`

	# ref. two condition check
	# https://stackoverflow.com/questions/16203088/bash-if-statement-with-multiple-conditions-throws-an-error
	if [ "$taskname" != "${array[5]}" ] || [ "$taskpid" != "${array[6]}" ]; then
		echo "prev ${array[1]} ${array[2]}, next ${array[5]} ${array[6]}"
		echo "next task at first line is must be $taskname, remove it"
		# ref. remove first line of file
		# https://stackoverflow.com/questions/4881930/remove-the-last-line-from-a-file-in-bash
		dd if=/dev/null of=$1 bs=1 seek=$(echo $(stat --format=%s $1) - $( head -n1 $1| wc -c) | bc )
		exit 1
	fi

	IFS=',' read -r -a array <<< `tail -n1 $1`

	# last line check
	if [ "$taskname" != "${array[1]}" ] || [ "$taskpid" != "${array[2]}" ]; then
		echo "prev ${array[1]} ${array[2]}, next ${array[5]} ${array[6]}"
		echo "prev task at last line is must be $taskname, remove it"
		dd if=/dev/null of=$1 bs=1 seek=$(echo $(stat --format=%s $1) - $( tail -n1 $1| wc -c) | bc )
		exit 1
	fi
}

check_n_fix_data()
{
	# temporarily, we dont use passed tasklist
	#for filename in `cat $1 | tr ',' '.' `; do
	for filename in `ls -S output/pertask | head -n20`; do
		linenr=`cat output/pertask/$filename | wc -l`
		if [ $(( $linenr % 2)) -ne 0 ]; then
			echo "$filename is odd"
			#fix_data output/pertask/$filename
		fi
	done
}

trans_to_csv()
{

	echo "Make ($FILELIST) to csv style"
	mkdir -p output
	rm -rf output/*.tasklist

	for filename in $FILELIST; do
		get_only_switch_field $filename
		make_it_csv "output/$filename.only_switch"
	done
}

merge_cpuusage_of_pertask()
{
	echo `ls -S $1` | tr ' ' ',' > output/tot_cpuusage_pertask
	# ref. https://unix.stackexchange.com/questions/53737/how-to-list-all-files-ordered-by-size
	# https://unix.stackexchange.com/questions/185227/using-find-to-list-all-files-under-certain-directory/

	paste `find output/pertask/elapsed/ -type f -exec du -h {} + | sort -r -h | cut -f2` >> output/tot_cpuusage_pertask
}


do_all_stage()
{
	trans_to_csv
	find_all_taskname_percpu
	get_uniq_tasklist
	split_event_into_pertask "output/tasklist"
	check_n_fix_data "output/tasklist"
	get_cpuusage_for_pertask "output/tasklist"
	merge_cpuusage_of_pertask "output/pertask/elapsed"
}

get_process_detail_evt()
{
	if [ ! "$1" ]; then
		echo "need to pass *pid* argument"
		exit 1
	fi

	if [ ! "$2" ]; then
		echo "There is no input file argument, use ./processes.txt"
		INPUT="processes.txt"
	fi

	if [ ! "$3" ]; then
		echo "There is no output file argument, use process.<PID>.stat"
		OUTPUT="process.$pid.stat"
	fi

	echo -n "Paring data - "
	cat $INPUT | grep $PID | tr -s " " > output/tmp.pick_n_trim
	cat output/tmp.pick_n_trim | cut -d' ' -f14 > output/tmp.threadlist
	cat output/tmp.threadlist | sort | uniq > output/tmp.threadlist.uniq

	TASK_LIST=`cat output/tmp.threadlist.uniq`

	for task in $TASK_LIST; do
		CNT_PREEMPTION=`cat cpu* | grep prev_comm=$task | grep prev_state=R | wc -l`
		CNT_YIELD=`cat cpu* | grep prev_comm=$task | grep -v prev_state=R | wc -l`
		echo $task $CNT_PREEMPTION $CNT_YIELD >> output/$OUTPUT
	done
	echo "Done"

	ls -al output/$OUTPUT
}

while [[ $# -gt 0 ]]; do
	argument="$1"
	case $argument in
                --pid) PID="$2"; echo "option - targeted Process ID($PID)"; shift; shift; ;;
		--mode) OP_MODE=$2; echo "option - mode($OP_MODE)"; shift; shift; ;;
		--debug) DEBUG=$2; shift; shift; ;;
		--input) INPUT="$2"; echo "option - input($INPUT)"; shift; shift; ;;
		--output) OUTPUT="$2"; echo "option - output($OUTPUT)"; shift; shift; ;;
		*) echo "Unknown option ($1)"; exit 1; ;;
	esac
done

if [ "$OP_MODE" == "all" ]; then
	do_all_stage
elif [ "$OP_MODE" == "csv" ]; then
	trans_to_csv
elif [ "$OP_MODE" == "tasklist" ]; then
	find_all_taskname_percpu
elif [ "$OP_MODE" == "tasklist_uniq" ]; then
	get_uniq_tasklist
elif [ "$OP_MODE" == "split_event" ]; then
	split_event_into_pertask "output/tasklist"
elif [ "$OP_MODE" == "check_n_fix" ]; then
	check_n_fix_data "output/tasklist"
elif [ "$OP_MODE" == "get_cpuusage" ]; then
	get_cpuusage_for_pertask "output/tasklist"
elif [ "$OP_MODE" == "gcu" ]; then
	get_cpuusage_for_task $INPUT
elif [ "$OP_MODE" == "merge_cpuusage" ]; then
	merge_cpuusage_of_pertask $INPUT
elif [ "$OP_MODE" == "get_ypt" ]; then
	get_process_detail_evt $PID $INPUT $OUTPUT
else
	do_all_stage
fi
