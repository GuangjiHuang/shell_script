#! /bin/bash
get_string=$(python ~/opt/myscript/change-dir/dir_forwards.py $@)
time_usage=${get_string#*#}
forward_path=${get_string%#*}
echo "-> ${forward_path} [$time_usage]"
cd "$forward_path"
