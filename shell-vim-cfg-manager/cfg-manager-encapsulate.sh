#! /bin/bash

# the gobal var of the path
# modules
modules=("vim" "bash" "tmux" "input")
# source path
script_source_path=~/mygithub/shell-script/shell-vim-cfg-manager
cfg_source_path=~/mygithub/shell-vim-cfg
# install path
common_install_path=~
vim_install_path=~/.vim
backup_path=$cfg_source_path/cfg-backup

# check if the backup_path exists
if [ ! -d $backup_path ]; then
    mkdir -p $backup_path
fi

# check the arguments and the path
checkArgPath() {
    if [ -d $cfg_source_path/$1 ];then
        cd $cfg_source_path
    else
        echo "$cfg_source_path/$1 not exists!"
    fi

    if [ ! $2 ];then
        echo "No the argument2! You can use the <install, uninstall, update>!"
    fi
}

# object + operation function (for example vim install)
objOperation(){
    # call the check function
    checkArgPath $1 $2
    for file_or_dir in $(ls -a $1)
    do
        if [[ $file_or_dir =~ \.$ ]]; then
            continue
        fi
        # for the operation
        case $2 in 
        "install")
            echo "----- Install: "
            # for the ~/.vim/hgj-vim-config
            if [ "$1" == "vim" ] && [ -d $1/$file_or_dir ];then
                rm -r $vim_install_path/$file_or_dir
                cp -r $1/$file_or_dir $vim_install_path/$file_or_dir
                echo "$1/$file_or_dir -> $vim_install_path/$file_or_dir"
                continue
            fi
            cp -r $1/$file_or_dir $common_install_path/$file_or_dir
            echo "$1/$file_or_dir -> $common_install_path/$file_or_dir"
            ;;
        "uninstall")
            echo "----- Uninstall: "
            # for the ~/.vim/hgj-vim-config
            if [ "$1" == "vim" ] && [ -d $1/$file_or_dir ];then
                rm -r $vim_install_path/$file_or_dir 
                echo "remove: $vim_install_path/$file_or_dir"
                continue
            fi
            rm -r $common_install_path/$file_or_dir 
            echo "remove: $common_install_path/$file_or_dir"
            ;;
        "update")
            echo "----- Update: "
            # for the ~/.vim/hgj-vim-config
            if [ "$1" == "vim" ] && [ -d $vim_install_path/$file_or_dir ];then
                cp -r $vim_install_path/$file_or_dir ./$1
                echo "$vim_install_path/$file_or_dir -> $(pwd)/$1"
                continue
            fi
            if [ -e $common_install_path ];then
                cp -r $common_install_path/$file_or_dir ./$1
                echo "$common_install_path/$file_or_dir -> $(pwd)/$1"
            fi
            ;;
        "backup")
            echo "------ Backup"
            # for the ~/.vim/hgj-vim-config
            if [ "$1" == "vim" ] && [ -d $vim_install_path/$file_or_dir ];then
                cp -r $vim_install_path/$file_or_dir $backup_path/$1/
                echo "$vim_install_path/$file_or_dir -> $backup_path/$1"
                continue
            fi
            if [ -e $common_install_path ];then
                cp -r $common_install_path/$file_or_dir $backup_path/$1/
                echo "$common_install_path/$file_or_dir -> $backup_path/$1" 
            fi
            ;;
        "restore")
            echo "------ Restore"
            echo "To restore the files, Pass!"
            ;;
        *)
            echo "$2 is not the valid argument. Use the install/uninstall/update/backup/restore please!"
            ;;

        esac
    done
}

case $1 in
    "vim" | "bash" | "tmux" | "input")
        
        # call the function
        objOperation $1 $2
        ;;

    "all")
        for module in ${modules[@]}
        do
            objOperation $module $2
            echo "-> $2 $module. Ok" 
        done
        ;;

    "go-cfg")
        if [ -d $cfg_sorce_path ];then
            cd $cfg_source_path
            echo "-> $cfg_source_path"
        else
            echo "No $cfg_sour_path, please <git clone git://github.com/GuangjiHuang/shell-vim-cfg $cfg_source_path>"
        fi
        ;;

    "go-manager")
        cd $script_source_path
        echo "-> $script_source_path"
        ;;

    "cfg")
        vim $script_source_path/cfg-manager-encapsulate.sh
        ;;

    "reinstall")
        cwd=$(pwd)
        cd $script_source_path
        ./install.sh
        cd $cwd
        echo "Has been reinstalled!"
        ;;

    "help")
        echo "this is the help information!"
        echo "-> vim/bash/tmux/input/all install/uninstall/backup/update/restore"
        echo "-> go-cfg: go to the cfg dir"
        echo "-> go-manager: go the shell-vim-cfg-manager!"
        echo "-> cfg: vim xxx.sh"
        echo "-> help"
        ;;

    *)
        echo "No argument input! Please type use the <help> option to get more inprmation!"
        ;;
esac
