#!/bin/sh

for port_nr in `seq 10000 10100`; do
	echo $port_nr
	iptables -t filter -I INPUT 1 -p tcp --dport $port_nr -j ACCEPT
	iptables -t nat -A PREROUTING -i eth1 -p tcp -d 192.168.105.100 --dport $port_nr -j DNAT --to-destination 172.16.0.2:$port_nr
	iptables -t nat -A POSTROUTING -o hv0 -j MASQUERADE
done
