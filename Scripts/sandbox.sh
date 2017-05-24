#!/bin/bash
xdotool mousemove 0 0
echo Starting SARndbox ...
/home/sandbox/src/SARndbox-2.3r2/bin/SARndbox -uhm -fpv -wts 320 240 -rs 0.7 -rer 20 100 "$@"
echo Done
