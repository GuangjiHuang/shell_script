#! /bin/bash

# set the command name you want
command_name=mygit
link_dir=/usr/bin

# the path to deal with
source_name="git_manager.sh"
source_dir=$(pwd)
source_path=$(pwd)/$source_name
install_dir=/opt/myscript/git-manager


## check if there is the same name command, using the which command
#if [[ "$(which ${command_name})" == /.* ]]; then
#    echo "${RED}ERROR${NOCOLOR}: Fail to install! Has been existed: $(which ${command_name})"
#    exit
#fi

# check if the install path, and then delete
if [ -d ${install_dir} ]; then
    rm -r ${install_dir}
fi
cp -r $(pwd) /opt/myscript/ && echo "Has been copied the directory to the: /opt/myscript"

# use the ln command to creat the link
ln -s ${install_dir}/${source_name} ${link_dir}/${command_name}

# check if link successfully
which ${command_name} >/dev/null

#if [ -n "$(which ${command_name})" ]; then
if [ $? -eq 0 ];then
    echo "Successfully! The executable file is: $(which ${command_name})"
fi

