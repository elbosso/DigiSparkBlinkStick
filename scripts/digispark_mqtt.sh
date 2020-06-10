#!/bin/bash

# Absolute path to this script. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in. /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")

host=mqtt.pi-docker.lab
port=1883
topicpattern=home/blinkstick/#


while :
do
echo "Listening for topic(s): $topicpattern"
msg=$(mosquitto_sub -h "$host" -p "$port" -t "$topicpattern" -v -C 1)
# if you must provide credentials in order to be able to connect to your
# broker, comment out the last line and uncomment the next three - taking
# care to set user and password to correct values!
#user=mosquitto
#password=password
#msg=$(mosquitto_sub -h "$host" -p "$port" -t "$topicpattern" -u "$user" -P "$password" -v -C 1)
echo "Message: $msg"
topic=$(echo -n "$msg" | cut -d ' ' -f 1)
lastpart=$(echo -n "$topic" |rev |cut -d '/' -f 1|rev) 
payload=$(echo -n "$msg" | cut -d ' ' -f 2)
echo "Topic: $topic"
echo "Topic (last part): $lastpart"
echo "Payload: $payload"

if [ "$lastpart" = "OK" ]; then 
	payload="green"
elif [ "$lastpart" = "warning" ]; then 
	payload="yellow"
elif [ "$lastpart" = "error" ]; then 
	payload="red"
elif [ "$lastpart" = "raw" ]; then 
	payload="$payload"
fi

"$SCRIPTPATH/digispark_rgb_led.sh" "$payload"

done
