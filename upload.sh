#!/bin/bash

FQBN=esp8266:esp8266:generic
PORTS=(/dev/tty.usbserial*)

echo "Select serial port:"
select port in "${PORTS[@]}"; do 
   break
done

arduino-cli upload -b $FQBN -p $port WifiRelay.ino
