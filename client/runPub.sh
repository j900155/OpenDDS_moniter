#! /bin/bash
testNumber=10
topic="A"
ID=2
usec=10000
sed -e  "1c${topic}" -i config.conf
sed -e  "3c${usec}" -i config.conf
for ((i=0; i<${testNumber}; i++))
do
	echo ${i}
	sed -e  "2c${i}" -i config.conf
	./publisher -DCPSConfigFile rtps.ini &
	sleep 1
done
