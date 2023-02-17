#! /bin/bash
forward_path=$(python /opt/myscript/change-dir/dir_forwards.py $@)
echo "-> ${forward_path}"
cd $forward_path
