#!/bin/bash

host=mqtt.pi-docker.lab
port=1883
topicpattern="home/blinkstick/raw"

OPTIONS=(0 "off" 1 "turquois" 2 "red" 3 "green" 4 "blue" 5 "purple" 6 "notwhite" 7 "yellow" 8 "brightpink" 9 "chartreuse" a "cycle" b "lime" c "continuous")

#echo $OPTIONS

CHOICE=$(dialog --clear --backtitle "Backtitle here" --title "Title here" --menu "Choose one of the following options:" 15 40 0 "${OPTIONS[@]}" 2>&1 >/dev/tty)

echo "$CHOICE"

if [ -z "$CHOICE" ]; then
echo "cancelled"
else
echo "$CHOICE"
case "$CHOICE" in
        1)
            mosquitto_pub  -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "turquois"
            ;;
        2)
            mosquitto_pub  -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "red"
            ;;
        3)
            mosquitto_pub  -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "green"
            ;;
        4)
            mosquitto_pub  -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "blue"
            ;;
        5)
            mosquitto_pub  -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "purple"
            ;;
        6)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "notwhite"
            ;;
        7)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "yellow"
            ;;
        8)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "brightpink"
            ;;
        9)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "chartreuse"
            ;;
        a)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "c"
            ;;
        b)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "lime"
            ;;
        c)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "M"
            ;;
        0)
            mosquitto_pub -h "$host" -p "$port" -t "$topicpattern" -i "$HOSTNAME" -m "off"
            ;;
esac
fi

clear