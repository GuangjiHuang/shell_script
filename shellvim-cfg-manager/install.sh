#! /bin/bash

# set the command name you want
command_name=cfg
install_path=/usr/bin
source_dir=shellvim-cfg-manager
install_dir_path=/opt/myscript

# copy the directory to the /opt/myscritp/
if [ -d "$install_dir_path/$source_dir" ]; then
    rm -r $install_dir_path/$source_dir
fi

cp -r $(pwd) $install_dir_path && echo "Has been copied the directory to the: /opt/myscript"

# use the ln command to creat the link
ln -s $install_dir_path/$source_dir/cfg-manager-encapsulate.sh ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "Successfully! The executable file is: $(which ${command_name})"
fi
