# DigiSparkBlinkStick
Software for controlling a 
[Digispark USB Gadget](http://digistump.com/products/1)
with a 
[RGB-Led Shield](http://digistump.com/products/118)
via serial connection

<!---
[![start with why](https://img.shields.io/badge/start%20with-why%3F-brightgreen.svg?style=flat)](http://www.ted.com/talks/simon_sinek_how_great_leaders_inspire_action)
--->
[![GitHub release](https://img.shields.io/github/release/elbosso/DigiSparkBlinkStick/all.svg?maxAge=1)](https://GitHub.com/elbosso/DigiSparkBlinkStick/releases/)
[![GitHub tag](https://img.shields.io/github/tag/elbosso/DigiSparkBlinkStick.svg)](https://GitHub.com/elbosso/DigiSparkBlinkStick/tags/)
[![GitHub license](https://img.shields.io/github/license/elbosso/DigiSparkBlinkStick.svg)](https://github.com/elbosso/DigiSparkBlinkStick/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/elbosso/DigiSparkBlinkStick.svg)](https://GitHub.com/elbosso/DigiSparkBlinkStick/issues/)
[![GitHub issues-closed](https://img.shields.io/github/issues-closed/elbosso/DigiSparkBlinkStick.svg)](https://GitHub.com/elbosso/DigiSparkBlinkStick/issues?q=is%3Aissue+is%3Aclosed)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/elbosso/DigiSparkBlinkStick/issues)
[![GitHub contributors](https://img.shields.io/github/contributors/elbosso/DigiSparkBlinkStick.svg)](https://GitHub.com/elbosso/DigiSparkBlinkStick/graphs/contributors/)
[![Github All Releases](https://img.shields.io/github/downloads/elbosso/DigiSparkBlinkStick/total.svg)](https://github.com/elbosso/DigiSparkBlinkStick)
[![Website elbosso.github.io](https://img.shields.io/website-up-down-green-red/https/elbosso.github.io.svg)](https://elbosso.github.io/)

## Getting started
Just as soon as you connect the gadget with any USB port, the Led on top will start to dimly glow blue. At that time,
the PC should add a new serial Device. Then you can send commands to the device by simply sending messages over
this serial device. _scripts/digispark_rgb_led.sh_ is a script making this easier under Linux. For a similar Windows batch file
I look forward to receiving merge requests. 

It is possible to control the color of the LED as well as the behavior: should it shine continuously or should it blink.

## Project organization

The master branch has the source code for the whole thang using a simple dumb RGB led, the branch NeoPixel is aptly named - here resides the code for when you want to use a smart RGB led...

## How to use

There is a small bash script _scripts/digispark_rgb_led.sh_ you can use to
control the led. This is very basic, but also documents the various possible
syntaxes to send to achieve different behaviours.

Another small bash script _scripts/digispark_mqtt.sh_ build upon this 
script and makes it possible to react on mqtt event. This is only a rough
sketch meant to act as foundation for your own ideas.

## Message Syntax and Semantics

### Names of colors

The predefinied colors are:

* turquois (#6)
* red (#1)
* green (#2)
* blue (#4)
* purple (#5)
* notwhite (#7)
* yellow (#3)
* off (#0)
* brightpink
* chartreuse

Those names are only understood by the shell scripts - if you try and send
them directly to the hardware, nothing will happen. The hardware only
understands the syntaxes given below:

### Aliases

In the list of names above, there are numbers behind some of them: these act
as aliases. There are 8 different aliases (the numbers from 0 to 7). The
number is interpreted binary - so you essentially express which color
component (red, green, blue) should be switched on (corresponding bit == 1)
or
off (corresponding bit == 0) with the bits meaning:

* 0 - red
* 1 - green
* 2 - blue

Before each alias there must be a designator. There are at the moment only
two distinctive ones:
* # solid
* + blink

So sending `#3` would mean solid glowing blue and `#1` would mean blink
green.

### RGB

There is also the possibility to specify an intensity for each color
component rather than just switching them on and off. For that you must give
the intensities for each component with exactly three digits, separated by
commas. For example: `255,255,000`. As with the aliases, one must prepend
every one of these Strings with designators. In the case of RGB values,
those and their meanings are:

* \* solid
* ! blink
