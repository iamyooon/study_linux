#!/bin/sh

MONITORING_START_RSS=""
PATH_LEAK_OUT_START="/ccos/apps/hmi/hudhmi/leaks.out.start"
PATH_LEAK_OUT_END="/ccos/apps/hmi/hudhmi/leaks.out.end"
PATH_LEAK_OUT_CP_START="/log_data/hudhmi.leaks.out.start"
PATH_LEAK_OUT_CP_END="/log_data/hudhmi.leaks.out.end"
PATH_WORKING_DIR="/rw_data/hudhmi.leakinfo"

cp_proc_meminfo()
{
	PID="$1"
	POSTFIX="$2"
	cp /proc/$PID/maps $PATH_WORKING_DIR/hudhmi.$PID.maps.$POSTFIX
	cp /proc/$PID/smaps $PATH_WORKING_DIR/hudhmi.$PID.smaps.$POSTFIX
	cp /proc/$PID/status $PATH_WORKING_DIR/hudhmi.$PID.staus.$POSTFIX
}

clean_prev_files()
{

	filelist="/rw_data/hudhmi.status \
		/rw_data/hudhmi.smaps \
		/log_data/hudhmi.* \
		/ccos/apps/hmi/hudhmi/leaks.out"

	for file in $filelist; do
		if [ -f $file ]; then
			rm $file
		fi
	done
}

env_setup()
{
	echo "wewake - setup env"

	mount -o remount,rw /

	systemctl stop hudhmi

	cp /ccos/apps/hmi/hudhmi/hudhmi /ccos/apps/hmi/hudhmi/hudhmi.org
	cp hudhmi /ccos/apps/hmi/hudhmi/hudhmi
	chown ap_hud:xgroup /ccos/apps/hmi/hudhmi/hudhmi
	chmod 777 /ccos/apps/hmi/hudhmi/hudhmi

	cp /ccos/apps/hmi/hudhmi/start.sh /ccos/apps/hmi/hudhmi/start.sh.org
	cp start.sh /ccos/apps/hmi/hudhmi/start.sh
	chmod 777 start.sh

	cp libleaktracer.so /usr/lib

	mkdir $PATH_WORKING_DIR

	sync

	ls -al /ccos/apps/hmi/hudhmi/hudhmi \
		/ccos/apps/hmi/hudhmi/start.sh \
		/usr/lib/libleaktracer.so

	systemctl restart hudhmi

	echo "wewake - setup test"

	grep "leaktracer" /proc/`pidof hudhmi`/maps
	if [ "$?" == "1" ]; then
		echo "wewake - setup failed, leaktracer.so no loaded"
		exit -1
	fi

	start_and_report_leak "hudhmi"
	test_monitoring_work_or_not 3
	clean_prev_files

	echo "wewake - setup done"
}

restore()
{
	echo "wewake - restore"

	mount -o remount,rw /

	systemctl stop hudhmi

	echo "wewake - before"
	ls -al /ccos/apps/hmi/hudhmi/hudhmi* /ccos/apps/hmi/hudhmi/start.sh*

	mv /ccos/apps/hmi/hudhmi/hudhmi.org /ccos/apps/hmi/hudhmi/hudhmi
	mv /ccos/apps/hmi/hudhmi/start.sh.org /ccos/apps/hmi/hudhmi/start.sh
	sync

	echo "wewake - after"
	ls -al /ccos/apps/hmi/hudhmi/hudhmi* /ccos/apps/hmi/hudhmi/start.sh*

	systemctl restart hudhmi
}

test_monitoring_work_or_not()
{
	sleep $1

	if [ ! -f /ccos/apps/hmi/hudhmi/leaks.out ] ; then
		echo "wewake - report failed, leaks.out cannot created!!!"
		echo "wewake - signal is may blocked by hudhmi, please restart hudhmi(systemctl restart hudhmi)"
		exit -1
	fi

	echo "wewake - report success, leaks.out can created"
}

start_and_report_leak()
{
	process="$1"
	pid=`pidof $process`
	kill -n 12 $pid
	kill -n 30 $pid
}

mon_start()
{
	END_RSS="$1"
	START_RSS="$2"
	MONITORING_STARTED="0"

	start_pid=`pidof hudhmi`
	echo "wewake - start memleak monitor script"

	mount -o remount,rw /
	clean_prev_files
	cp_proc_meminfo "$start_pid" "start"

	while true; do
		pid=`pidof hudhmi`
		current_mem=`ps -Ao comm,pid,rss | grep hudhmi | tr -s ' ' | cut -d " " -f3`

		echo "StartPID($start_pid),CurrentPID($pid),RSS($current_mem)"

		cp_proc_meminfo "$pid" "last"

		if [ "$MONITORING_STARTED" == "0" ]; then
			if [ "$current_mem" -gt "$START_RSS" ]; then
				echo "wewake - Start to collect memory allocation info from now"
				echo "wewake - current rss($current_mem) is greater than start_rss($START_RSS)"
				start_and_report_leak "hudhmi"
				test_monitoring_work_or_not 3
				clean_prev_files
				MONITORING_STARTED="1"
			fi
		elif [ "$current_mem" -gt "$END_RSS" ]; then
			echo "wewake - hudhmi memleak occured!!!"

			kill -n 30 $pid
			cp_proc_meminfo "$pid" "end"

			ls -al $PATH_WORKING_DIR \
				/ccos/apps/hmi/hudhmi/leaks.out
			exit 0
		fi
		sleep 10
	done
}

if [ "$1" == "setup" ]; then
	env_setup
elif [ "$1" == "restore" ]; then
	restore
else
	if [ "$1" == "" ]; then
		echo "wewake - invalid arguments"
		exit -1
	fi

	if [ "$2" == "" ]; then
		echo "wewake - Start rss value is not given. set startrss as 0"
		MONITORING_START_RSS="0"
	else
		MONITORING_START_RSS="$2"
	fi

	mon_start "$1" "$MONITORING_START_RSS"
fi
