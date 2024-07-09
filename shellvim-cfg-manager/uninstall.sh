#! /bin/bash

# set the command name you want
command_name=cfg
install_path=~/usr/bin
source_dir=shellvim-cfg-manager
install_dir_path=/opt/myscript

# remove the link in the install_path
rm $install_path/$command_name

# remove the $install_dir_path/$source_dir
rm -r $install_dir_path/$source_dir

echo "Successfully remove the $command_name!"
