#! /bin/bash
file="/opt/myscript/wr-script/font/*"
while [ 1 ]
do

    for i in $file
    do
        file_name=${i##*/}
        if [ ${file_name:0:5} == "year-" ]; then
            #cat $i && echo $(datef) 
            cat $i 
            sleep 2
            clear
        fi
    done
done

