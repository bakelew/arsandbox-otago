#!/bin/bash

# Kill all of the potentially conflicting processes
# -w wait until they are killed, -q be quiet about it
killall -w -q CalibrateProjector
killall -w -q SARndbox
killall -w -q KinectUtil

# Reset all the Kinect devices
# KinectUtil reset all
KinectUtil reset all > /dev/null
