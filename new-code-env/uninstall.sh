#! /bin/bash

# set the cmmand name you want
script_dir=new-code-env
command_name=code
install_path=/usr/bin

# remove the link in the install_path
rm ${install_path}/${command_name}

# remove the /opt/myscript/p-script
rm -r /opt/myscript/${script_dir}/

echo "Successfully remove the ${script_dir}!"
