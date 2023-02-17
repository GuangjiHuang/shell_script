#!/bin/bash

# set the command name you want
script_dir=back-dir
command_name=b
install_path=/usr/bin

#copy the directory to the /opt/myscritpt
if [ -d "/opt/myscript/${script_dir}" ]; then
    rm -r /opt/myscript/${script_dir}
fi
cp -r $(pwd) /opt/myscript/ && echo "--Has been copied the directory to the: /opt/myscript"

# use the ln command to creat the link
ln -s "/opt/myscript/back-dir/dir_backwards.sh" ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "--Successfully! The executable file is: $(which ${command_name})"
fi
