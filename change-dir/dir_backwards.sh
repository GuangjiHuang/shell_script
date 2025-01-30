#! /bin/bash
B_SRC_PATH=~/mygithub/shell-script/change-dir
B_DST_PATH=~/opt/myscript/change-dir

if [ "$1" == "go-src" ]; then
	cd $B_SRC_PATH
elif [ "$1" == "go-dst" ]; then
	cd $B_DST_PATH
else
	back_path=$(python ~/opt/myscript/change-dir/dir_backwards.py $1)
	cd $back_path
fi
