#!/bin/bash

host=mqtt.pi-docker.lab
port=1883
topicpattern="home/blinkstick/raw"

DELAY=$(dialog --inputbox "Delay in minutes..." 0 0 2>&1 >/dev/tty);

echo "$DELAY"

if [ -z "$DELAY" ]; then
echo "cancelled"
else
echo "$DELAY"
echo 'mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i $(tr -dc A-Za-z0-9 </dev/urandom | head -c 13) -m "+5"' |at now +"$DELAY" min
fi
