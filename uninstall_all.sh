#! /bin/bash

# use the find to find the xxx/xxx/uninstall.sh
base_path=$(pwd)

for i in $(find . -name "uninstall.sh")
do
    # cd to the uninstall.sh's dir
    dst_path=$base_path/$i
    dst_dir=${dst_path%/*}

    cd $dst_dir
    echo "-> $(pwd)"
    ./uninstall.sh
done
