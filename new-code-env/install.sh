#!/bin/bash

# set the command name you want
script_dir=new-code-env
command_name=code
install_path=$HOME/usr/bin

#copy the directory to the $HOME/opt/myscritpt
if [ -d "$HOME/opt/myscript/${script_dir}" ]; then
    rm -r $HOME/opt/myscript/${script_dir}
fi
cp -r $(pwd) $HOME/opt/myscript/ && echo "--Has been copied the directory to the: $HOME/opt/myscript"
# use the ln command to creat the link
ln -s "$HOME/opt/myscript/${script_dir}/${command_name}.sh" ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "--Successfully! The executable file is: $(which ${command_name})"
fi
