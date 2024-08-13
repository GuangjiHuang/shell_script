#! /bin/bash

# remove the link in the install_path
rm $HOME/usr/bin/wr

# remove the $HOME/opt/myscript/p-script
rm -r $HOME/opt/myscript/wr-script/

echo "Successfully remove the wr-script!"
