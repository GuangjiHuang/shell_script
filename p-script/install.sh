#!/bin/bash

# set the command name you want
script_dir=p-script
command_name=p
install_path=/usr/bin

# if exists the record-path.txt, and then backup it
if [ -f "/opt/myscript/p-script/record-path.txt" ]; then
    if [ ! -d /opt/myscript/backup ];then
        mkdir -p /opt/myscript/backup/
    fi
    cp /opt/myscript/p-script/record-path.txt /opt/myscript/backup/
    echo "--backup successfully!"
fi
#copy the directory to the /opt/myscritpt
if [ -d "/opt/myscript/p-script" ]; then
    rm -r /opt/myscript/p-script
fi
cp -r $(pwd) /opt/myscript/ && echo "--Has been copied the directory to the: /opt/myscript"
# then copy the record-path.txt to the file from the backup
if [ -f "/opt/myscript/backup/record-path.txt" ]; then
    cp /opt/myscript/backup/record-path.txt /opt/myscript/p-script/
    echo "--recover the record-path.txt sucessfully!"
fi
# use the ln command to creat the link
ln -s "/opt/myscript/p-script/path.sh" ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "--Successfully! The executable file is: $(which ${command_name})"
fi
