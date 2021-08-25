#!/bin/sh

PATH_LEAK_OUT="/ccos/apps/hmi/hudhmi/leaks.out"

leaktracer_setup()
{
	echo "wewake - leaktracer setup"

	cp libleaktracer.so /usr/lib
	sync
}

hudhmi_setup()
{
	echo "wewake - hudhmi setup"

	systemctl stop hudhmi

	cp /ccos/apps/hmi/hudhmi/hudhmi /ccos/apps/hmi/hudhmi/hudhmi.org
	cp hudhmi /ccos/apps/hmi/hudhmi/hudhmi
	chown ap_hud:xgroup /ccos/apps/hmi/hudhmi/hudhmi
	chmod 777 /ccos/apps/hmi/hudhmi/hudhmi

	cp /ccos/apps/hmi/hudhmi/start.sh /ccos/apps/hmi/hudhmi/start.sh.org
	cp start.sh /ccos/apps/hmi/hudhmi/start.sh
	chmod 777 start.sh

	leaktracer_setup

	sync

	ls -al /ccos/apps/hmi/hudhmi/hudhmi /ccos/apps/hmi/hudhmi/start.sh /usr/lib/libleaktracer.so

	systemctl restart hudhmi
}

start_leaktracer()
{
	echo "wewake - start leaktracer"

	kill -n 12 `pidof hudhmi`
}

capture_leak_report()
{
	echo "wewake - capture $1 leak report"

	if [ -f $PATH_LEAK_OUT ]; then
		rm $PATH_LEAK_OUT
	fi
	kill -n 30 `pidof hudhmi`
	sleep 3
}

hudhmi_setup_test()
{
	echo "wewake - hudhmi setup test"

	grep "leaktracer" /proc/`pidof hudhmi`/maps
	if [ "$?" == "1" ]; then
		echo "wewake - setup failed, leaktracer.so no loaded"
	fi

	start_leaktracer
	capture_leak_report test

	if [ ! -f $PATH_LEAK_OUT ] ; then
		echo "wewake - setup failed, leaks.out cannot created"
	else
		rm $PATH_LEAK_OUT
	fi

	echo "wewake - hudhmi setup test success"
}

env_setup()
{

	echo "wewake - setup env"

	mount -o remount,rw /

	hudhmi_setup
	hudhmi_setup_test
}

restore()
{
	echo "wewake - restore"

	mount -o remount,rw /

	systemctl stop hudhmi
	mv /ccos/apps/hmi/hudhmi/hudhmi.org /ccos/apps/hmi/hudhmi/hudhmi
	mv /ccos/apps/hmi/hudhmi/start.sh.org /ccos/apps/hmi/hudhmi/start.sh
	sync

	ls -al /ccos/apps/hmi/hudhmi/hudhmi /ccos/apps/hmi/hudhmi/start.sh
	systemctl restart hudhmi
}

mon_start()
{
	echo "wewake - start memleak monitor"

	mount -o remount,rw /

	rm /rw_data/hudhmi.status
	rm /rw_data/hudhmi.smaps
	rm /log_data/hudhmi.*

	start_leaktracer
	capture_leak_report start

	pid=`pidof hudhmi`

	cp /proc/$pid/maps /log_data/hudhmi.maps.start
	cp $PATH_LEAK_OUT /log_data/hudhmi.leaks.out.start

	while true; do
		pid=`pidof hudhmi`

		cat /proc/$pid/status >> /rw_data/hudhmi.status
		cat /proc/$pid/smaps >> /rw_data/hudhmi.smaps

		mem=`ps -Ao comm,pid,rss | grep hudhmi | tr -s ' ' | cut -d " " -f3`

		echo "PID($pid),RSS($mem)"

		if [ "$mem" -gt "$1" ]; then
			echo "wewake - hudhmi memleak occured!!!"

			capture_leak_report issue
			cp /proc/$pid/maps /log_data/hudhmi.maps.end
			cp $PATH_LEAK_OUT /log_data/hudhmi.leaks.out.end

			ls -al /log_data/hudhmi.*
			ls -al /rw_data/hudhmi.*
			echo "wewake - memleak monitor end"
			exit
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
