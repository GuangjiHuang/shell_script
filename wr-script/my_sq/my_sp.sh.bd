#! /bin/bash
file="/opt/myscript/wr-script/my_sq/*"
ctr_command=""
while [ ! $ctr_command ]
do

    for i in $file
    do
        file_name=${i##*/}
        if [ ${file_name:0:3} == "sq_" ]; then
            cat $i && echo $(datef) 
        fi
        # get the control command
        read -t 1 ctr_command 
        if [  $ctr_command ]; then
            break
        fi
        # 
        #sleep 1
        clear
    done
done

