#!/bin/bash
function HELP()
{
	local args=$1
	local prom_str=$2
	echo -e "${GREEN}: ${args}${NOCOLOR} ${prom_str}"
	echo
}

P_NAME="path.sh"
P_DST=~/opt/myscript/p-script
P_SRC=~/mygithub/shell-script/p-script

record_path=$P_DST/record-path.txt
tmp_record_path=$P_DST/tmp_record_path.txt

num_lines=$(sed -n '1p' $record_path)

if [ $# -lt 1 ]
then
    echo "Please add the args!"
    return
fi

case "$1" in
l)
    cat $record_path
    ;;
w)
    let num_lines=$num_lines+1
    echo "${num_lines}. $(pwd)" >> $record_path
    echo "${num_lines}. $(pwd) >> $record_path"
    sed -i "1c${num_lines}" $record_path
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
    vim $P_SRC/$P_NAME
    ;;
t)
	cd $(cat $tmp_record_path)
	;;
T)
	echo $PWD > $tmp_record_path
	;;
[1-9]*)
    if [[ $1 =~ ^[1-9][0-9]{0,1}$ ]];then
        let lines=$1+1
        s_path=$(sed -n "${lines}p" $record_path)
        cd_path=${s_path#* }
        #export cd_path
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
"go-src")
	cd $P_SRC
    ;;
"go-dst")
	cd $P_DST
    ;;
"reinstall")
	cp $P_SRC/$P_NAME $P_DST/$P_NAME
	;;
help)
    echo -e "${YELLOW}-------------------------------> OPTION <---------------------------------------${NOCOLOR}"
	HELP "c" "clear all the record path"
	HELP "l" "list the path you record"
	HELP "m" "manmually change the record file opened using vim"
	HELP "p" "vim $P_NAME"
	HELP "w" "write the path to the record list"
	HELP "t" "cd to the tmp path"
	HELP "T" "store the tmp path"
	HELP "[1-9]" "chose the path you want to go"
	HELP "[1-9] q" "cd with the quite mode, use to copy file to it"
	HELP "go-src" "go to the src dir"
	HELP "go-dst" "go to the dst dir"
	HELP "help" "to show the command's option"
    echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
    ;;
*)
    echo "Error: you can use the command <p help> to get more information"
    ;;
esac
