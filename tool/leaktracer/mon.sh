#!/bin/sh

PATH_LEAK_OUT_START="/ccos/apps/hmi/hudhmi/leaks.out.start"
PATH_LEAK_OUT_END="/ccos/apps/hmi/hudhmi/leaks.out.end"
PATH_LEAK_OUT_CP_START="/log_data/hudhmi.leaks.out.start"
PATH_LEAK_OUT_CP_END="/log_data/hudhmi.leaks.out.end"
PATH_WORKING_DIR="/rw_data/hudhmi.leakinfo"

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
	echo "wewake - start memleak monitor"
	mount -o remount,rw /

	clean_prev_files
	start_and_report_leak "hudhmi"
	test_monitoring_work_or_not 3
	clean_prev_files

	cp /proc/$pid/maps $PATH_WORKING_DIR/hudhmi.$pid.maps.start
	cp /proc/$pid/smaps $PATH_WORKING_DIR/hudhmi.$pid.smaps.start
	cp /proc/$pid/status $PATH_WORKING_DIR/hudhmi.$pid.staus.start

	while true; do
		pid=`pidof hudhmi`
		mem=`ps -Ao comm,pid,rss | grep hudhmi | tr -s ' ' | cut -d " " -f3`

		echo "PID($pid),RSS($mem)"

		cp /proc/$pid/maps $PATH_WORKING_DIR/hudhmi.$pid.maps.last
		cp /proc/$pid/smaps $PATH_WORKING_DIR/hudhmi.$pid.smaps.last
		cp /proc/$pid/status $PATH_WORKING_DIR/hudhmi.$pid.staus.last

		if [ "$mem" -gt "$1" ]; then
			echo "wewake - hudhmi memleak occured!!!"

			kill -n 30 $pid

			cp /proc/$pid/maps $PATH_WORKING_DIR/hudhmi.$pid.maps.end
			cp /proc/$pid/smaps $PATH_WORKING_DIR/hudhmi.$pid.smaps.end
			cp /proc/$pid/status $PATH_WORKING_DIR/hudhmi.$pid.staus.end

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
	mon_start "$1"
fi
