#!/bin/bash

# set the command name you want
install_path=$HOME/usr/bin

#copy the directory to the $HOME/opt/myscritpt
if [ -d "$HOME/opt/myscript/change-dir" ]; then
    rm -r $HOME/opt/myscript/change-dir
fi
cp -r $(pwd) $HOME/opt/myscript/ && echo "--Has been copied the directory to the: $HOME/opt/myscript"

# use the ln command to creat the link
ln -s "$HOME/opt/myscript/change-dir/dir_backwards.sh" ${install_path}/b
ln -s "$HOME/opt/myscript/change-dir/dir_forwards.sh" ${install_path}/f
ln -s "$HOME/opt/myscript/change-dir/collect_path.sh" ${install_path}/c

# check if link successfully
if [ -n "$(which b)" ]; then
    echo "--Successfully! The executable file is: $(which b)"
fi

if [ -n "$(which f)" ]; then
    echo "--Successfully! The executable file is: $(which f)"
fi

if [ -n "$(which c)" ]; then
    echo "--Successfully! The executable file is: $(which c)"
fi
