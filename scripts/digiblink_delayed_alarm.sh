#!/bin/bash

host=mqtt.pi-docker.lab
port=1883
topicpattern="home/blinkstick/raw"

minutes=5
hours=0
days=0
weeks=0


VALUES=$(dialog --form "Delay" 0 0 0 "Delay in minutes..." 1 1 "$minutes" 1 20 10 0 "Delay in hours" 3 1 "$hours" 3 20 10 0 "Delay in days" 5 1 "$days" 5 20 10 0 "Delay in weeks" 7 1 "$weeks" 7 20 10 0  2>&1 >/dev/tty)
rv=$?
echo $rv
echo "values $VALUES"
responsearray=($VALUES)
minutes=responsearray[0]
hours=responsearray[1]
days=responsearray[2]
weeks=responsearray[3]


if [ "$rv" = "1" ]; then
echo "cancelled"
else
echo "hours $hours"
effectiveminutes=$((minutes + $hours * 60 + $days * 60 * 24 + weeks * 60 * 24 * 7 ))
echo "effective $effectiveminutes"
rv=$(echo "mosquitto_pub -h $host -p $port -t $topicpattern -i $(tr -dc A-Za-z0-9 </dev/urandom | head -c 13) -m '+5'" |at now +"$effectiveminutes" min 2>&1|grep job) 
#echo "job: $rv"
jobnumber=$(echo -n $rv|cut -d ' ' -f 2)
#atqq=$(atq)
dialog --msgbox "Job created:\n$rv" 0 0
#dialog --msgbox "job number: $jobnumber" 0 0
details=$(at -c "$jobnumber")
dialog --msgbox "$details" 0 0
fi

clear

