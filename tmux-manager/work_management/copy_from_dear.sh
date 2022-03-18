#! /bin/bash

scp -r $dear_pc:/home/ysq/code/c++/work_management/{*.cpp,*.h,*.sh,*.txt,py-script,data} .
if [ $? -eq 0 ]; then
    clear
fi
