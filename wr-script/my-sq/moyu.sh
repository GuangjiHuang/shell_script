#! /bin/bash
file="/opt/myscript/wr-script/my-sq/*"
while [ 1 ]
do

    for i in $file
    do
        file_name=${i##*/}
        if [ ${file_name:0:5} == "moyu-" ]; then
            cat $i && echo $(datef) 
            sleep 1
            clear
        fi
    done
done

