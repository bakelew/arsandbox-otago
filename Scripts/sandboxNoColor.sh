#!/bin/bash
xdotool mousemove 0 0
echo Starting SARndbox ...
/home/sandbox/src/SARndbox-2.3r2/bin/SARndbox -uhm NoColorMap.cpt -fpv -rs 0.7 -rer 20 100 "$@"
echo Done
