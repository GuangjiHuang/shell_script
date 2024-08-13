#! /bin/bash

# set the command name you want
command_name=wr
install_path=$HOME/usr/bin

## check if there is the same name command, using the which command
#if [[ "$(which ${command_name})" == /.* ]]; then
#    echo "${RED}ERROR${NOCOLOR}: Fail to install! Has been existed: $(which ${command_name})"
#    exit
#fi

# copy the directory to the $HOME/opt/myscritp/
if [ -d "$HOME/opt/myscript/wr-script" ]; then
    rm -r $HOME/opt/myscript/wr-script
fi
# change the date-pointer to today that you are installing the wr-script
day_pointer=$(date "+%Y-%m/%m-%d")
echo $day_pointer > date-pointer.txt

cp -r $(pwd) $HOME/opt/myscript/ && echo "Has been copied the directory to the: $HOME/opt/myscript"

# use the ln command to creat the link
ln -s $HOME/opt/myscript/wr-script/write.sh ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "Successfully! The executable file is: $(which ${command_name})"
fi

