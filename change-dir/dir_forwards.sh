#! /bin/bash

F_SRC_PATH=~/mygithub/shell-script/change-dir
F_DST_PATH=~/opt/myscript/change-dir

if [ "$1" == "go-src" ]; then
	cd $B_SRC_PATH
elif [ "$1" == "go-dst" ]; then
	cd $B_DST_PATH
else
	get_string=$(python ~/opt/myscript/change-dir/dir_forwards.py $@)
	time_usage=${get_string#*#}
	forward_path=${get_string%#*}
	echo "-> ${forward_path} [$time_usage]"
	cd "$forward_path"
fi
