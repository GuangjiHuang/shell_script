C_SRC_PATH=~/mygithub/shell-script/change-dir
C_DST_PATH=~/opt/myscript/change-dir

python_script_path=$C_DST_PATH/collect_path.py
pickle_file_path=$C_SRC_PATH/most-often-used-path.pkl

current_dir=$(pwd)

if [[ "$1" =~ ^[0-9]*$ ]]; then
    python $python_script_path $@
elif [ "$1" == "clear" ]; then
	if [ -f $pickle_file_path ]; then
		rm $pickle_file_path
	fi
    echo "rm the pikle path: $pickle_file_path"
elif [ "$1" == "view" ]; then
    vim $(python $python_script_path "view")
elif [ "$1" == "go-src" ]; then
	cd $C_SRC_PATH
elif [ "$1" == "go-dst" ]; then
	cd $C_DST_PATH
elif [ "$1" == "reinstall" ]; then
	cp $C_SRC_PATH/*.{sh,py} $C_DST_PATH
else
    get_string=$(python $python_script_path $@)
    time_usage=${get_string#*#}
    forward_path=${get_string%#*}
    echo "-> ${forward_path} [$time_usage]"
    cd "$forward_path"
    # if change dir successfully list the dir
    if [ "$(pwd)" != "$current_dir" ]; then
        ls -alh
    fi
fi
