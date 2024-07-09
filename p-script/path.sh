#!/bin/bash
record_path=~/opt/myscript/p-script/record-path.txt
num_lines=$(sed -n '1p' $record_path)
#echo "the numlines is : ${num_lines}"
if [ $# -lt 1 ]
then
    echo "Please add the args!"
    return
fi
case "$1" in
l)
    # list the path in the $record_path
    cat $record_path
    ;;
w)
    # add the path to the record-path.txt
    let num_lines=$num_lines+1
    echo "${num_lines}. $(pwd)" >> $record_path
    echo "${num_lines}. $(pwd) >> $record_path"
    # renew the num_lines
    sed -i "1c${num_lines}" $record_path # sed command, -i is the option; 1 is the adress, c is the command, ${num_lines} is the text";
    ;;
c)
    read -p "Are you sure to clear all the record path? (y/n): " key_input
    if [ $key_input == "y" ]; then
        echo "0" > $record_path 
    else
        echo "Cancel to clear the record path!"
    fi
    ;;
m)
    vim ${record_path}
    ;;
p)
    p go-p
    vim path.sh
    ;;
help)
    echo -e "${YELLOW}-------------------------------> OPTION <---------------------------------------${NOCOLOR}"

    echo -e "${GREEN}: c->clear${NOCOLOR} all the record path;"
    echo
    echo -e "${GREEN}: go-p${NOCOLOR} -> go the path.sh's directory"
    echo 
    echo -e "${GREEN}: help->help${NOCOLOR} to show the command's option;"
    echo
    echo -e "${GREEN}: l->list${NOCOLOR} the path you record;"
    echo
    echo -e "${GREEN}: m->manmually${NOCOLOR} changed the record file opened using vim;"
    echo
    echo -e "${GREEN}: p${NOCOLOR} -> vim path.sh"
    echo
    echo -e "${GREEN}: w->write${NOCOLOR} the path to the rocord list;"
    echo
    echo -e "${GREEN}: [1-9]->chose${NOCOLOR} the path you want to go." 
    echo
    echo -e "${GREEN}: [1-9] q->cd with quiet mode${NOCOLOR} the quite mode, use to copy file to it"
    echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
    ;;
[1-9]*)
    if [[ $1 =~ ^[1-9][0-9]{0,1}$ ]];then # the regular expression matching
        let lines=$1+1
        s_path=$(sed -n "${lines}p" $record_path)
        #cd_path=${s_path: 3}
        cd_path=${s_path#* }
        #export PATH=$PATH:${cd_path}
        export cd_path
        if [ "$2" == "q" ]; then
            echo ${cd_path}
            else
            echo "-> ${cd_path}"
            cd ${cd_path}
        fi
    else
        echo "Error: you can use the command <p help> to get more information"
    fi
    ;;
"go-p")
    # check if the file is existed?
    dst_path_1="/home/hgj/mygithub/shell-script/p-script/"
    dst_path_2="~/opt/myscript/p-script/"
    if [ -d ${dst_path_1} ]; then
        cd ${dst_path_1}
        echo "-> ${dst_path_1}"
    elif [ -d $dst_path_2 ]; then
        cd ${dst_path_2}
        echo "-> ${dst_path_2}"
    else
        echo -e "${RED}Error:${NOCOLOR} no the path.sh in your filesystem!"
    fi
    ;;
*)
    echo "Error: you can use the command <p help> to get more information"
    ;;
esac
