#! /bin/bash
dir_path=/opt/myscript/tmux-manager/work_management
. ${dir_path}/main.exe ${dir_path}/data

if [ ! $? -eq 0 ]; then # can not execute
    cd $dir_path
    . run.sh
fi
