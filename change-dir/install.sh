#!/bin/bash

# set the command name you want
install_path=$HOME/usr/bin

change_dir_dst_path=$HOME/opt/myscript/change-dir

B_PATH=$HOME/opt/myscript/change-dir/dir_backwards.sh
F_PATH=$HOME/opt/myscript/change-dir/dir_forwards.sh
C_PATH=$HOME/opt/myscript/change-dir/collect_path.sh


if [ -d $change_dir_dst_path ]; then
    rm -r $change_dir_dst_path
fi
cp -r $(pwd) $change_dir_dst_path &&\
echo "cp $PWD -> $change_dir_dst_path"

if [ -f $install_path/b ]; then
	rm $install_path/b
fi

if [ -f $install_path/f ]; then
	rm $install_path/f
fi

if [ -f $install_path/c ]; then
	rm $install_path/c
fi

ln -s $B_PATH ${install_path}/b
ln -s $F_PATH ${install_path}/f
ln -s $C_PATH ${install_path}/c

if [ -n "$(which b)" ]; then
    echo "--Successfully! The executable file is: $(which b)"
fi

if [ -n "$(which f)" ]; then
    echo "--Successfully! The executable file is: $(which f)"
fi

if [ -n "$(which c)" ]; then
    echo "--Successfully! The executable file is: $(which c)"
fi
