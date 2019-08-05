#!/bin/bash
INPUTFILE=""
OUTPUTFILE="result"
OUTPUTDIR=""

if [ $# -lt 1 ]; then
        echo "need to argument such as input/$OUTPUTDIR file"
        exit 1
fi

while [[ $# -gt 0 ]]; do
        argument="$1"
        case $argument in
                --input) INPUTFILE="$2"; echo "option - input($INPUTFILE)"; shift; shift; ;;
                --output) OUTPUTFILE="$2"; echo "option - $OUTPUTFILE($OUTPUTFILE)"; shift; shift; ;;
                *) echo "Unknown option ($1)"; exit 1; ;;
        esac
done

if [ ! "$INPUTFILE" ]; then
        echo "need to pass *input file* argument"
        exit 1
fi

OUTPUTDIR="output."`basename $INPUTFILE`

if [ -d "$OUTPUTDIR" ]; then
        echo "Remove DIR $OUTPUTDIR"
        rm -rf $OUTPUTDIR
fi

echo "Making DIR $OUTPUTDIR"
mkdir $OUTPUTDIR

echo -n "Paring data - "
# grep only sched_switch event
cat $INPUTFILE | grep sched_switch > $OUTPUTDIR/sched_switch
# make it as list separated by blank
cat $OUTPUTDIR/sched_switch | sed 's/: / /g;s/=/ /g;s/ > //g' | tr -s " " | sed 's/^ //g' > $OUTPUTDIR/sched_switch.formatted

#get column(assume that kernel use fixed width)
cat $OUTPUTDIR/sched_switch | cut -c'-21' | tr -s " " > $OUTPUTDIR/column_current-taskpid
cat $OUTPUTDIR/sched_switch | cut -c'24-28' | tr -s " " > $OUTPUTDIR/column_cpu
cat $OUTPUTDIR/sched_switch | cut -c'34-46' | tr -s " " > $OUTPUTDIR/column_timestamp
cat $OUTPUTDIR/sched_switch | cut -d':' -f2 | tr -s " " > $OUTPUTDIR/column_eventname
cat $OUTPUTDIR/sched_switch | cut -c'62-' | sed 's/prev_comm=//g;s/prev_pid=/,/g;s/prev_prio=/,/g;s/prev_state=/,/g;s/==> next_comm=/,/g;s/next_pid=/,/g;s/next_prio=/,/g' | sed 's/ ,/,/g' > $OUTPUTDIR/sched_switch.formatted2
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f1 | sed 's/ //g' > $OUTPUTDIR/column_currname
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f2 > $OUTPUTDIR/column_currpid
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f3 > $OUTPUTDIR/column_currprio
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f4 > $OUTPUTDIR/column_currstate
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f5 | sed 's/ //g' > $OUTPUTDIR/column_nextname
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f6 > $OUTPUTDIR/column_nextpid
cat $OUTPUTDIR/sched_switch.formatted2 | cut -d',' -f7 > $OUTPUTDIR/column_nextprio

#get {curr, next}+{name,pid}
paste $OUTPUTDIR/column_currname $OUTPUTDIR/column_currpid | sed 's/\t/-/g' > $OUTPUTDIR/curr_name+pid
paste $OUTPUTDIR/column_nextname $OUTPUTDIR/column_nextpid | sed 's/\t/-/g' > $OUTPUTDIR/next_name+pid

# create task top list
cat $OUTPUTDIR/curr_name+pid | sort | uniq -c | sort -bnr > $OUTPUTDIR/tasktop

# create data list for checking cpu usage
#cat $OUTPUTDIR/sched_switch.formatted | cut -d' ' -f4,7,9,15,17 > $OUTPUTDIR/sched_switch.formatted.for_cpuusage
paste -d' ' $OUTPUTDIR/curr_name+pid $OUTPUTDIR/column_timestamp $OUTPUTDIR/next_name+pid | sed 's/^ //g' > $OUTPUTDIR/sched_switch.formatted.for_cpuusage

echo "Done"
