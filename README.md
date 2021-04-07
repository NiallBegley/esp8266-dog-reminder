# esp8266-dog-reminder
## Overview

This is a very simple application for the ESP8266 that reminds me that my dog is outside by periodically sending push notifications to my phone.

Press a button to awake the ESP8266 and once awoken the device will periodically send a push notification to the given PushOver account.  You flip a switch to put the device into deep sleep mode once done.

I made this project for the Adafruit Feather HUZZAH ESP8266 using the Arduino IDE.  Here is a very crude circuit diagram that corresponds to the IO pins in the source.

![schematic](https://user-images.githubusercontent.com/6789600/113876193-7ee03700-9785-11eb-9b85-967e0867b8ea.png)
