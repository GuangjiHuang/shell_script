#! /bin/bash
while [ 1 ]
do
    tmux clock
    sleep 2
    tmux send-keys 'q' C-m
    #sleep 10

done
