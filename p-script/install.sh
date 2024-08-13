#!/bin/bash

# set the command name you want
script_dir=p-script
command_name=p
install_path=$HOME/usr/bin

# if exists the record-path.txt, and then backup it
if [ -f "$HOME/opt/myscript/p-script/record-path.txt" ]; then
    if [ ! -d $HOME/opt/myscript/backup ];then
        mkdir -p $HOME/opt/myscript/backup/
    fi
    cp $HOME/opt/myscript/p-script/record-path.txt $HOME/opt/myscript/backup/
    echo "--backup successfully!"
fi
#copy the directory to the $HOME/opt/myscritpt
if [ -d "$HOME/opt/myscript/p-script" ]; then
    rm -r $HOME/opt/myscript/p-script
fi
cp -r $(pwd) $HOME/opt/myscript/ && echo "--Has been copied the directory to the: $HOME/opt/myscript"
# then copy the record-path.txt to the file from the backup
if [ -f "$HOME/opt/myscript/backup/record-path.txt" ]; then
    cp $HOME/opt/myscript/backup/record-path.txt $HOME/opt/myscript/p-script/
    echo "--recover the record-path.txt sucessfully!"
fi
# use the ln command to creat the link
ln -s "$HOME/opt/myscript/p-script/path.sh" ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "--Successfully! The executable file is: $(which ${command_name})"
fi
