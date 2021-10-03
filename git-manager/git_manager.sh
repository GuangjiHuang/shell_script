#! /bin/bash

#---------- the var----------  
# the reposities
reposities=("linux_basic_cfg"\
            "shell_script"\
            "program_language_test"\
            "goal"\
            "everyday_record"\
            "_station_"
            "tmux_treasure") # note: there may has the private reposity

# the path
source_dir="${HOME}/mygithub/shell_script/git-manager/"
source_path="${HOME}/mygithub/shell_script/git-manager/git_manager.sh"
install_dir="/opt/myscript/git-manager/"

# checkout the path
flag_source_exist=0
flag_install_exist=0
if [ -f $source_path ];then
    flag_source_exist=1
fi
if [ -f $source_path ];then
    flag_install_exist=1
fi
#------------------------------

#---------- the command----------  
# the git list
case $1 in
    "list")
        order=1
        echo "----------- reposities ---------------"
        for i in ${reposities[@]}
        do
            echo "${order}->: $i"
            let order=order+1
        done
        echo
        ;;

# the git clone
    "clone")
        # loop the arguments
        for reposity in $@
        do
            # skip the $1(that is the clone)
            if [ $reposity == "clone" ];then
                continue
            fi
            echo ${reposities[@]} | grep -wq "$reposity" && flag="is"
            if [ $flag == "is" ];then
                echo "------------- git the ${reposity} ---------------"
                #git clone git://github.com/GuangjiHuang/${reposity}.git >/dev/null
                git clone git@github.com:GuangjiHuang/${reposity}.git >/dev/null
                if [ $? -eq 0 ];then
                    echo "------------- Successfully ---------------"
                else
                    echo "Fail!"
                fi
            else
                echo "Note:---------------------"
                echo "$reposity not found!"
                echo "Please refer to the follow: "
                echo ${reposities[@]}
            fi
        done
        ;;

# the git clone all
    "clone-all")
        for reposity in ${reposities[@]}
        do
            echo "------------- git the ${reposity} ---------------"
            git clone git://github.com/GuangjiHuang/${reposity}.git >/dev/null
            if [ $? -eq 0 ];then
                echo "------------- Successfully ---------------"
            else
                echo "Fail!"
            fi
        done
        ;;
    # the git add_reposity
    "add-reposity")
        # just vim the this file
        echo "You are modifying the source file, remember to install again after modifing!"
        if [ $flag_source_exist -eq 1 ];then
            vim $source_path
        else
            echo "$source_path not exists!"
        fi
        ;;
# go to the source
    "go-source")
        cd $source_dir
        echo "->:$(pwd)"
        ;;

# go to the install
    "go-install")
        cd $install_dir
        echo "->:$(pwd)"
        ;;

# add the content, then commit, then push
    "push")
        # get the commit content
        if [ -n "$2" ]; then
            commit_content="$2"
        else
            commit_content="update->$(date "+%Y-%m-%d %a %X")"
        fi
        # the git command
        echo "------ try to git push  ------"
        # remember to add the double quote to the ${commit_content}, otherwise it will make error
        git add . && git commit -m "${commit_content}" && git push
        # check if the command succeed
        if [ $? ]; then
            echo "------ git push successfully! ------"
        else
            echo "------ git push fail ------"
        fi
        ;;

# to install the script
    "reinstall")
        # cd to the source dir and then excute the install.sh
        cd ${source_dir} && ./install.sh
        ;;

# the help
    "help")
        clear
        echo -e "${YELLOW}-------------------------------> OPTION <---------------------------------------${NOCOLOR}"
        echo -e "${GREEN}: add-reposity->:${NOCOLOR} manually to edit the source file to add the reposities"
        echo
        echo -e "${GREEN}: clone x y ...->:${NOCOLOR} to clone the reposities(x, y ... is the reposity)"
        echo
        echo -e "${GREEN}: clone-all->:${NOCOLOR} clone all the reposities"
        echo
        echo -e "${GREEN}: go-source->:${NOCOLOR} go to the source directory"
        echo
        echo -e "${GREEN}: go-install->:${NOCOLOR} go to the install directory"
        echo
        echo -e "${GREEN}: list->:${NOCOLOR} list all the reposities"
        echo 
        echo -e "${GREEN}: help->:${NOCOLOR} show the help information"
        echo
        echo -e "${GREEN}: push->:${NOCOLOR} git add . && git commit -m xxx && git push"
        echo
        echo -e "${GREEN}: reinstall->:${NOCOLOR} reinstall the source file to cover the original file"
        echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
        ;;

    *)
        echo "Error: argument < $@ > not exists!"
        echo "You can use the arugment \"help\" to see more option!"
        ;;

esac
