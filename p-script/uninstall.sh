#! /bin/bash

# remove the link in the install_path
rm ~/usr/bin/p

# backup the record-path.txt
if [ -f "~/opt/myscript/p-script/record-path.txt" ]; then
    cp ~/opt/myscript/p-script/record-path.txt ~/opt/myscript/backup/
fi

# remove the ~/opt/myscript/p-script
rm -r ~/opt/myscript/p-script/

echo "Successfully remove the p-script!"
