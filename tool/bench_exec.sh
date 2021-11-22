#!/bin/sh

work_dir="./log"
device_id=""
adb_dev_num=""
log_cnt=0
id_cnt=0

run_bench_tool()
{
	device_id=$1
	echo "- Run the bench tool on $device_id"
	adb -s $device_id shell sysbench --test=cpu --num-threads=4 --cpu-max-prime=9999 run | grep "total time" > $work_dir/result/$device_id
}

start_bench()
{
	adb devices >> $work_dir/adb_devices.log
	adb_dev_num=`cat $work_dir/adb_devices.log | grep device -w | wc -l`
	if [ $adb_dev_num -lt 1 ]; then
		echo "No target board connected"
		exit
	else
		echo "> $adb_dev_num target boards connected"
		cat $work_dir/adb_devices.log | grep device -w | cut -f1 | while read id
		do
			echo "- device id: $id"
			./bench_exec.sh run $id
			echo $id >> $work_dir/id_list
		done
	fi
}

wait_done()
{
	echo "Wait bench test done"
	id_cnt=`cat $work_dir/id_list | wc -l`
	log_cnt=`ls -l log/result/ | grep ^- | wc -l`
	while [ $log_cnt -lt $id_cnt ]
	do
		echo "log_cnt=$log_cnt, id_cnt=$id_cnt"
		sleep 1
		id_cnt=`cat $work_dir/id_list | wc -l`
		log_cnt=`ls -l log/result/ | grep ^- | wc -l`
	done

	cat $work_dir/id_list | while read id
	do
		while true;
		do
			log_line_num=`cat $work_dir/result/$id | wc -l`
			if [ $log_line_num -gt 0 ]; then
				break;
			else
				sleep 1
			fi
		done
	done
}

print_result()
{
	echo "Print bench test result"
	cat $work_dir/id_list | while read id
	do
		echo "- device id: $id"
		cat $work_dir/result/$id
	done
}

bench()
{
	echo "Start bench test"
	rm -rf $work_dir
	mkdir $work_dir
	mkdir $work_dir/result

	start_bench
	wait_done
	sleep 3
	print_result
}


if [ "$1" == "run" ]; then
	run_bench_tool $2 $3&
else
	bench
fi
