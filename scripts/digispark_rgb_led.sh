#!/bin/bash
# shellcheck disable=SC2162
#
# the predefinied colors are:
# turquois (#6)
# red (#1)
# green (#2)
# blue (#4)
# purple (#5)
# notwhite (#7)
# yellow (#3)
# off (#0)
# brightpink
# chartreuse
#
# the user can freely define colors from a
# palette with 256*256*256 colors
# to do so, one command line argument
# in the form of *rrr,ggg,bbb
# must be given - for example
# *255,030,000 to get a darkish orange 
color="$1"
if [ "$color" = "turquois" ]; then 
	color="*000,190,255"
elif [ "$color" = "red" ]; then 
	color="*255,000,000"
elif [ "$color" = "green" ]; then 
	color="*000,255,000"
elif [ "$color" = "blue" ]; then 
	color="*000,000,255"
elif [ "$color" = "purple" ]; then 
	color="*255,000,190"
elif [ "$color" = "notwhite" ]; then 
	color="*255,255,255"
elif [ "$color" = "yellow" ]; then 
	color="*255,060,000"
elif [ "$color" = "off" ]; then 
	color="*000,000,000"
elif [ "$color" = "brightpink" ]; then 
	color="*255,020,060"
elif [ "$color" = "chartreuse" ]; then 
	color="*208,045,000"
elif [ "$color" = "lime" ]; then 
	color="*255,165,000"
fi
grep -o . <<< "$color"|while read letter;
do 
#from ls -l /dev/serial/by-id
echo -n "$letter" >/dev/serial/by-id/usb-digistump.com_Digispark_Serial-if00
#echo -n "$letter" >/dev/ttyACM0
# this line can be activated if the digispark does not seem to pick up on
# the commands - chances are that the communication rate is to fast for it
# then...
#sleep 0.01
done
