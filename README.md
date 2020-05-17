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
