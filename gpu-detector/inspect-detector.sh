#! /bin/bash

# the var
#script_path=/home/scut/ftp/gpu-detector.sh
script_path=./gpu-detector.sh
#log_path=/home/scut/ftp/detect.log
log_path=./detect.log
# check the gpu-detector.sh if run
ps -ef | grep gpu-detector | grep -v grep
if [ ! "0" == "$?" ]; then 
    echo "$(date) Status: off" >> $log_path
    . $script_path
else
    echo "$(date) Status: run" >> $log_path
fi
