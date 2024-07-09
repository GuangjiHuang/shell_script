#! /bin/bash

# set the command name you want
command_name=love
link_dir=~/usr/bin

# the path to deal with
source_name="tmux-manager.sh"
source_dir=$(pwd)
source_path=$(pwd)/$source_name
install_dir=~/opt/myscript/tmux-manager

# remove the link in the install_dir
rm ${link_dir}/${command_name}

# remove the install path
rm -r ${install_dir}

echo "Successfully remove the ${command_name}!"
