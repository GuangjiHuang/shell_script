#! /bin/bash

# set the command name you want
command_name=wr
install_path=~/usr/bin

## check if there is the same name command, using the which command
#if [[ "$(which ${command_name})" == /.* ]]; then
#    echo "${RED}ERROR${NOCOLOR}: Fail to install! Has been existed: $(which ${command_name})"
#    exit
#fi

# copy the directory to the ~/opt/myscritp/
if [ -d "~/opt/myscript/wr-script" ]; then
    rm -r ~/opt/myscript/wr-script
fi
# change the date-pointer to today that you are installing the wr-script
day_pointer=$(date "+%Y-%m/%m-%d")
echo $day_pointer > date-pointer.txt

cp -r $(pwd) ~/opt/myscript/ && echo "Has been copied the directory to the: ~/opt/myscript"

# use the ln command to creat the link
ln -s ~/opt/myscript/wr-script/write.sh ${install_path}/${command_name}

# check if link successfully
if [ -n "$(which ${command_name})" ]; then
    echo "Successfully! The executable file is: $(which ${command_name})"
fi

