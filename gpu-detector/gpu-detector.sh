#! /bin/bash

# the gobal var
gpu_usage_thres=20
rest_time_thres=60 # one minute
work_continue_time=0
rest_continue_time=0

# loop to detect the gpu status
ctn_num=0
while [ 1 ]
do
    # detect the usage of the gpu
    gpu_usage=$(nvidia-smi | grep -io [0-9][0-9]*%)
    gpu_usage=$(echo $gpu_usage)
    gpu_usage=${gpu_usage##* }
    gpu_usage=${gpu_usage%%\%}
    # compare the usage, > -> to sleep or count
    # check if meet the requrirement, if do something!
    if [ $gpu_usage -lt $gpu_usage_thres ];then
        let rest_continue_time+=1
        let work_continue_time=0
        sleep 1
    else
        let work_continue_time+=1
        let rest_continut_time=0
        sleep 1
    fi
    if [ $ctn_num -gt 59 ];then
        let ctn_num=0
        message="$(date) Usage: ${gpu_usage}" 
        if [ $rest_continue_time -gt $rest_time_thres ];then
            message="$message -> do something!"
        else
            message="$message -> waiting!"
        fi
        echo "$message" >> gpu-detect.log
    fi
    let ctn_num+=1
done
