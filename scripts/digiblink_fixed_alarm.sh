#!/bin/bash

host=mqtt.pi-docker.lab
port=1883
topicpattern="home/blinkstick/raw"

TIME=$(date "+%H %M %S")
tim=$(dialog --stdout --time-format "%H%M.%S" --title "Time" --timebox "Set the time:" 0 0 $TIME)


if [ -z "$tim" ]; then
echo "cancelled"
else
DATE=$(date "+%d %m %Y")
dat=$(dialog --stdout --date-format "%y%m%d" --title "Calendar" --calendar "At $tim on" 0 0 $DATE)
echo "$dat$tim"
if [ -z "$dat" ]; then
echo "cancelled"
else

#effectiveminutes=$((minutes + $hours * 60 + $days * 60 * 24 + weeks * 60 * 24 * 7 ))
#echo "effective $effectiveminutes"
rv=$(echo "mosquitto_pub -h $host -p $port -t $topicpattern -i $(tr -dc A-Za-z0-9 </dev/urandom | head -c 13) -m '+5'" |at -t "$dat$tim" 2>&1|grep job) 
#echo "job: $rv"
jobnumber=$(echo -n $rv|cut -d ' ' -f 2)
#atqq=$(atq)
dialog --msgbox "Job created:\n$rv" 0 0
#dialog --msgbox "job number: $jobnumber" 0 0
details=$(at -c "$jobnumber")
dialog --msgbox "$details" 0 0
fi
fi
clear

