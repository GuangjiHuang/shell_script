#! /bin/bash
python_script_path=/opt/myscript/change-dir/collect_path.py
pickle_file_path=~/mygithub/shell-script/change-dir/most-often-used-path.pkl
# judge the first argument is the digit
current_dir=$(pwd)
if [[ "$1" =~ ^[0-9]*$ ]]; then
    python $python_script_path $@
elif [ "$1" == "clear" ]; then
    rm $pickle_file_path
    echo "rm the pikle path: $pickle_file_path"
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
