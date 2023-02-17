#!/bin/bash

# set the command name you want
install_path=/usr/bin

#copy the directory to the /opt/myscritpt
if [ -d "/opt/myscript/change-dir" ]; then
    rm -r /opt/myscript/change-dir
fi
cp -r $(pwd) /opt/myscript/ && echo "--Has been copied the directory to the: /opt/myscript"

# use the ln command to creat the link
ln -s "/opt/myscript/change-dir/dir_backwards.sh" ${install_path}/b
ln -s "/opt/myscript/change-dir/dir_forwards.sh" ${install_path}/f

# check if link successfully
if [ -n "$(which b)" ]; then
    echo "--Successfully! The executable file is: $(which b)"
fi

if [ -n "$(which f)" ]; then
    echo "--Successfully! The executable file is: $(which f)"
fi
