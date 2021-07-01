#!/bin/bash
record_path=/opt/myscript/p-script/record_path.txt
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
# add the path to the record_path.txt
let num_lines=$num_lines+1
echo "${num_lines}. $(pwd)" >> $record_path
echo "${num_lines}. $(pwd) >> $record_path"
# renew the num_lines
sed -i "1c${num_lines}" $record_path
;;
c)
echo "0" > $record_path
;;
h)
    echo "======================================"
    echo -e "${GREEN}OPTION${NOCOLOR}"
    echo "======================================"

    echo -e ":l->list the path you record;\nw->write the path to the rocord list;\n:c->clear all the record path;\nh->help, to show the command's option;\n:m->manmually changed the record file opened using vim;\n:[1-9]->chose the path you want to go."
    ;;
1-9])
let lines=$1+1
s_path=$(sed -n "${lines}p" $record_path)
cd_path=${s_path: 3}
#export PATH=$PATH:${cd_path}
export cd_path
echo "-> ${cd_path}"
cd ${cd_path}
;;
*)
    echo "Error: you can use the command <p h> to get more information"
esac
