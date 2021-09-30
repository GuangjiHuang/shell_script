#! /bin/bash
# time: Mon Sep 27 14:49:05 2021

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# design and the function:

# 1) set up the different task and the layout. You just need to type the <mytmux task_name> to 
# enter the corresponding task; 
# if the task session has been created, then open it, else, create it and then open them! 
# -> the possible task may be the:
# c++; 
# python;
# vim;
# shell;
# markdown;
# project;
# paper;
# entertainment( to show the rest, moyu, fish picture ).

# 2) touch the task list for every task sessions. this task file is for record the task, or your 
# ideas or your thinking that you want to do something!

# 3) try to use more simple command to achieve that!

# >>>>>>>>>>>>>>>> the  tmux's layout <<<<<<<<<<<<<<<<<<<<<
tmux_c++()
{
    # new the session;
    # split the window;
    # resize the pane's size
    # send-keys to execute the commamd
    #|-----------------------------------------
    #|        0                      |   1    |
    #|                               |        |
    #|       edit                    |        |
    #|                               |  rain  |
    #|                               |        |
    #|       70%                     |        |
    #|                               |        |
    #|                               |  30%   |
    #|                               |        |
    #|-----------------------------------------
    tmux new-session -x 220  -y 55 -s "c++" \; \
    split-window -h \; \
    resize-pane -t 0 -x70% \; \
    send-keys -t 1 "v- c go;clear" C-m\; \
    send-keys -t 1 'wr rain' C-m\; \
    send-keys -t 0 "v- c go;clear" C-m\; \
    select-pane -t 0\;
}

tmux_python()
{
    echo "this is the python's tmux"
}

tmux_vim()
{
    # new the session;
    # split the window;
    # resize the pane's size
    # send-keys to execute the commamd
    #|-----------------------------------------
    #|   0     |           1         |   2    |
    #|         |                     |        |
    #|         |                     |        |
    #| rain    |         edit        |  rain  |
    #|         |                     |        |
    #|         |                     |        |
    #|         |     < maximum >     |        |
    #|         |                     |        |
    #|         |                     |        |
    #|-----------------------------------------

    tmux new-session -x 220  -y 55 -s "vim" \; \
    split-window -h \; \
    split-window -h \; \
    resize-pane -t 1 -x100% \; \
    send-keys -t 0 'wr rain' C-m\; \
    send-keys -t 2 'wr rain' C-m\; \
    send-keys -t 1 'v- v go;clear' C-m\; \
    select-pane -t 1\;
}

tmux_shell()
{
    echo "this is the shell's tmux"
}

tmux_markdown()
{
    echo "this is the markdown's tmux"
}

tmux_project()
{
    echo "this is the project's tmux"
}

tmux_paper()
{
    echo "this is the paper's tmux"
}

tmux_entertainment()
{
    echo "this is the entertainment's tmux"
}

# >>>>>>>>>>>>>>>> the main script <<<<<<<<<<<<<<<<<<<<<
# the var
sessions_list=("c++" \
    "python" \
    "vim" \
    "markdown" \
    "shell" \
    "paper" \
    "project" \
    "entertainment")
tmux_action="create"
source_dir=~/mygithub/shell_script/tmux-manager
source_name="${source_dir}/tmux-manager.sh"
install_dir=/opt/myscript/tmux-manager
# just check the session_list's argument
echo "${sessions_list[@]}" | grep -wq "$1" >/dev/null
if [ "$?" == 0 ];then
    # check the $1, make sure that into session or create the new session
    tmux ls 2>/dev/null | grep "$1" >/dev/null
    if [ "$?" == 0 ]; then
        tmux_action="into"
        echo " >> into $1"
    else
        tmux_action="create"
        echo " >> create $1"
    fi
fi

case "$1" in
    "c++")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_c++ 
        fi

        ;;
    "python")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_python
        fi
        ;;

    "vim")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_vim
        fi
        ;;

    "shell")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_shell
        fi
        ;;

    "markdown")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_markdown
        fi
        ;;

    "project")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_project
        fi
        ;;

    "paper")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_paper
        fi
        ;;

    "entertainment")
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_entertainment
        fi
        ;;
    "kill-all")
        tmux kill-server >/dev/null 2>&1
        if [ "$?" == 0 ];then
            echo "Successfully to kill all the sessions!"
        else
            echo "No tmux sessions to kill!"
        fi
        ;;

    "ls")
        # list all the existent sessions
        has_or_not=0
        mark_task_has_or_not="-"
        tmux ls > $install_dir/temp.txt 2>&1 && has_or_not=1
        if [ -z "$2" ] && [ "$has_or_not" == 1 ]; then
            while read -r line
            do
                echo ${line%%:*}
            done < $install_dir/temp.txt
        elif [ -z "$2" ] && [ "$has_or_not" == 0 ]; then
            echo "No tmux sessions!"
        elif [ "$2" == "all" ];then
            # check the task has or not
            for task in ${sessions_list[@]}
            do
                grep -qw $task $install_dir/temp.txt
                if [ "$?" == 0 ]; then
                    echo -e " (+) ${GREEN}$task${NOCOLOR}"
                else
                    echo -e " (-) $task"
                fi
            done
        else
            echo "I don't know what are you doing?"
        fi
        ;;

    "go-love")
        cd $source_dir
        echo "->:${source_dir}"
        ;;

    "go-install")
        cd $install_dir
        echo "->:${install_dir}"
        ;;

    "love")
        vim $source_name
        echo "Remember to execute the < love reinstall > to make it effect!"
        ;;

    "reinstall")
        # to install the file again to cover the original file
        love go-love
        ./install.sh && echo "----------------- reinstall successfully --------------------"
        ;;

    "help")
        clear
        echo -e "${YELLOW}-------------------------------> OPTION <---------------------------------------${NOCOLOR}"
        echo -e "${GREEN}: vim->:${NOCOLOR} create or into the vim tmux session"
        echo
        echo -e "${GREEN}: c++->:${NOCOLOR} create or into the c++ tmux session"
        echo
        echo -e "${GREEN}: python->:${NOCOLOR} create or into the python tmux session"
        echo
        echo -e "${GREEN}: markdown->:${NOCOLOR} create or into the markdown tmux session"
        echo
        echo -e "${GREEN}: shell->:${NOCOLOR} create or into the shell tmux session"
        echo
        echo -e "${GREEN}: entertainment->:${NOCOLOR} create or into the entertainment tmux session"
        echo
        echo -e "${GREEN}: project->:${NOCOLOR} create or into the project tmux session"
        echo
        echo -e "${GREEN}: paper->:${NOCOLOR} create or into the paper tmux session"
        echo
        echo -e "${GREEN}: kill-all->:${NOCOLOR} the same as the <tmux kill-server>, to kill all the sessions"
        echo
        echo -e "${GREEN}: go-love->:${NOCOLOR} go to the source dir: ~/mygithub/shell_script/..."
        echo
        echo -e "${GREEN}: go-install->:${NOCOLOR} go to the install dir: /opt/.../..."
        echo
        echo -e "${GREEN}: love->:${NOCOLOR} edit the tmux-manager.sh"
        echo
        echo -e "${GREEN}: ls->:${NOCOLOR} list all the existent sessions"
        echo
        echo -e "${GREEN}: help->:${NOCOLOR} show the help information"
        echo
        echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
        ;;
    *)
        echo "Error: No such tmux task!"
        ;;
esac
