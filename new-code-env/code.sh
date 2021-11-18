#! /bin/bash

# >>> design thinking
# 1) c/c++ code enviroment without the cmake
# -c
#
# 2) c/c++ code enviroment with the cmake
# -cm
# 
# 3) others
# -q : just to creat the new file not use the vim to open it!
# -code
# -go-code
# -reinstall
# -help
# <<< design thinking

# the gobal var
source_dir=~/mygithub/shell-script/new-code-env
install_dir=/opt/myscript/new-code-env
cwd=$(pwd)
for_simple_c_dir=${install_dir}/for_simple_c
for_cmake_c_dir=${install_dir}/for_cmake_c

# >>>>>>>>>>> ------ the case start
case $1 in
    "q")
        # call the python script to create the new file
        ## check if exist the touch_file.py
        if [ ! -f ./touch_file.py ];then
            # copy the file to this directory
            cp ${install_dir}/touch_file.py .
        fi
        new_file=$(python3 touch_file.py)
        echo "-> Has creat the new file: ${new_file}"
        ;;

    "c")
        if [ "$2" == "dir" ];then
            cp -r ${for_simple_c_dir} .
        else
            cp -r ${for_simple_c_dir}/* .
        fi
        ;;

    "cm")
        if [ "$2" == "dir" ];then
            cp -r ${for_cmake_c_dir} .
        else
            cp -r ${for_cmake_c_dir}/* .
        fi
        ;;

    "code")
        vim ${source_dir}/code.sh
        ;;

    "go-code")
        cd ${source_dir}
        ;;

    "go-install")
        cd ${install_dir}
        ;;

    "reinstall")
        cd ${source_dir}
        ./install.sh
        cd ${cwd}
        ;;

    "help")
        echo "This is the help information!"
        ;;

    "here")
        # call the python script to create the new file
        ## check if exist the touch_file.py
        if [ ! -f ./touch_file.py ];then
            # copy the file to this directory
            cp ${install_dir}/touch_file.py .
        fi

        new_file=$(python3 touch_file.py)
        # open the new file to write
        if [ "$?" == "0" ];then
            vim $new_file
        else
            echo "Fail to touch the new_file!"
            # if fail, delte the touch_file.py
            rm ./touch_file.py
        fi
        ;;
esac
