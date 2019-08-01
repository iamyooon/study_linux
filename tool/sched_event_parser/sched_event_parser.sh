#!/bin/bash

if [ ! "$1" ]; then
	echo "invalid arguments"
	exit 1
fi

mkdir -p output

# grep only sched_switch event
cat $1 | grep sched_switch > output/$1.sched_switch
# make it as list separated by blank
cat output/$1.sched_switch | sed 's/: / /g;s/=/ /g;s/ > //g' | tr -s " " | sed 's/^ //g' > output/$1.formatted
# create data list for checking cpu usage
cat output/$1.formatted | cut -d' ' -f4,7,9,15,17 > output/$1.raw.cpuusage

#get column(assume that kernel use fixed width)
cat output/$1.sched_switch | cut -c'-21' | tr -s " " > output/$1.column_current-taskpid
cat output/$1.sched_switch | cut -c'24-28' | tr -s " " > output/$1.column_cpu
cat output/$1.sched_switch | cut -c'34-46' | tr -s " " > output/$1.column_timestamp
cat output/$1.sched_switch | cut -d':' -f2 | tr -s " " > output/$1.column_eventname
cat output/$1.sched_switch | cut -c'62-' | sed 's/prev_comm=//g;s/prev_pid=/,/g;s/prev_prio=/,/g;s/prev_state=/,/g;s/==> next_comm=/,/g;s/next_pid=/,/g;s/next_prio=/,/g' | sed 's/ ,/,/g' > output/$1.sched_switch.formatted
cat output/$1.sched_switch.formatted | cut -d',' -f1 | sed 's/ //g' > output/$1.column_currname
cat output/$1.sched_switch.formatted | cut -d',' -f2 > output/$1.column_currpid
cat output/$1.sched_switch.formatted | cut -d',' -f3 > output/$1.column_currprio
cat output/$1.sched_switch.formatted | cut -d',' -f4 > output/$1.column_currstate
cat output/$1.sched_switch.formatted | cut -d',' -f5 | sed 's/ //g' > output/$1.column_nextname
cat output/$1.sched_switch.formatted | cut -d',' -f6 > output/$1.column_nextpid
cat output/$1.sched_switch.formatted | cut -d',' -f7 > output/$1.column_nextprio

#get {curr, next}+{name,pid}
paste output/$1.column_currname output/$1.column_currpid | sed 's/\t//g' > output/$1.curr_name+pid
paste output/$1.column_nextname output/$1.column_nextpid | sed 's/\t//g' > output/$1.next_name+pid

# create task top list
cat output/$1.curr_name+pid | sort | uniq -c | sort -bnr > output/$1.tasktop
