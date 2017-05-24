#!/bin/bash
xdotool mousemove 0 0
echo Starting CalibrateProjector ...
/home/sandbox/src/SARndbox-2.3r2/bin/CalibrateProjector -s 1280 800 "$@"
echo Done
