#!/bin/bash

script_dir=p-script
command_name=p
install_path=$HOME/usr/bin

if [ -f "$HOME/opt/myscript/p-script/record-path.txt" ]; then
    if [ ! -d $HOME/opt/myscript/backup ];then
        mkdir -p $HOME/opt/myscript/backup/
    fi
    cp $HOME/opt/myscript/p-script/record-path.txt $HOME/opt/myscript/backup/
    echo "--backup successfully!"
fi

if [ -d "$HOME/opt/myscript/p-script" ]; then
    rm -r $HOME/opt/myscript/p-script
fi
cp -r $(pwd) $HOME/opt/myscript/ && echo "--Has been copied the directory to the: $HOME/opt/myscript"

if [ -f "$HOME/opt/myscript/backup/record-path.txt" ]; then
    cp $HOME/opt/myscript/backup/record-path.txt $HOME/opt/myscript/p-script/
    echo "--recover the record-path.txt sucessfully!"
fi

if [ -f $install_path/$command_name ]; then
	rm $install_path/$command_name
fi

ln -s "$HOME/opt/myscript/p-script/path.sh" ${install_path}/${command_name}


if [ -n "$(which ${command_name})" ]; then
    echo "--Successfully! The executable file is: $(which ${command_name})"
fi
