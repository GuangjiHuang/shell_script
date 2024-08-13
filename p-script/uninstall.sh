#! /bin/bash

# remove the link in the install_path
rm $HOME/usr/bin/p

# backup the record-path.txt
if [ -f "$HOME/opt/myscript/p-script/record-path.txt" ]; then
    cp $HOME/opt/myscript/p-script/record-path.txt $HOME/opt/myscript/backup/
fi

# remove the $HOME/opt/myscript/p-script
rm -r $HOME/opt/myscript/p-script/

echo "Successfully remove the p-script!"
