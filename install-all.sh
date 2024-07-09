#! /bin/bash

# to fix the path.sh's install bug 
if [ ! -d ~/opt/myscript ];then
    mkdir ~/opt/myscript
fi
# use the find to find the xxx/xxx/install.sh
base_path=$(pwd)

for i in $(find . -name "install.sh")
do
    # cd to the install.sh's dir
    dst_path=$base_path/$i
    dst_dir=${dst_path%/*}

    cd $dst_dir
    echo "-> $(pwd)"
    ./install.sh
done
