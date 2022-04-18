#!/bin/bash

FQBN=esp8266:esp8266:generic

read -p "WIFI Network : " SSID
read -r -p "WIFI password: " -s PSK
echo
arduino-cli compile -b $FQBN  --build-property "compiler.cpp.extra_flags=\"-DSTASSID=\"$SSID\"\" \"-DSTAPSK=\"$PSK\"\"" -e WifiRelay.ino
