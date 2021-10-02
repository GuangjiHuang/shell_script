#! /bin/bash
# time: Mon Sep 27 14:49:05 2021

# >>>>>>>>>>>>>>>> the design explanation <<<<<<<<<<<<<<<<<<<<<
#{{{
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
#}}}

# >>>>>>>>>>>>>>>> the global variables <<<<<<<<<<<<<<<<<<<<<
#{{{
sessions_list=("c++" \
    "python" \
    "vim" \
    "markdown" \
    "shell" \
    "paper" \
    "project" \
    "entertainment" \
    "chore" \
    "English")
tmux_action="create"
source_dir=~/mygithub/shell_script/tmux-manager
source_name="${source_dir}/tmux-manager.sh"
install_dir=/opt/myscript/tmux-manager
mode_control_path=/opt/myscript/tmux-manager/mode_control.sh
#}}}

# >>>>>>>>>>>>>>>> the  tmux's layout <<<<<<<<<<<<<<<<<<<<<
#{{{
# 3 panes
tmux_template_3panes()
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
    
    # the parameters
    # $1 ->: the session's name
    tmux new-session -x 220  -y 55 -s "$1" \; \
    split-window -h \; \
    split-window -h \; \
    resize-pane -t 1 -x100% \; \
    send-keys -t 0 'wr rain' C-m\; \
    send-keys -t 2 'wr rain' C-m\; \
    send-keys -t 1 'v- v go;clear' C-m\; \
    select-pane -t 1\;

}

# 2 panes
tmux_template_2panes()
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

    # the parameters
    # $1: sessioin name
    # $2: left part big or the right part big
    if [ "$2" == "left_big" ];then
        part_percent=20%
    else
        part_percent=80%
    fi
    tmux new-session -x 220  -y 55 -s "$1" \; \
    split-window -h \; \
    resize-pane -t 0 -x$part_percent \; \
    send-keys -t 1 "v- c go;clear" C-m\; \
    send-keys -t 1 'wr rain' C-m\; \
    send-keys -t 0 "v- c go;clear" C-m\; \
    select-pane -t 0\;

}

tmux_c++()
{
    tmux_template_2panes "c++"
}

tmux_python()
{
    tmux_template_2panes "python" "left_big"
}

tmux_vim()
{
    tmux_template_3panes "vim"
}

tmux_shell()
{
    tmux_template_3panes "shell"
}

tmux_markdown()
{
    tmux_template_3panes "markdown"
    echo "this is the markdown's tmux"
}

tmux_project()
{
    tmux_template_2panes "project"
    echo "this is the project's tmux"
}

tmux_paper()
{
    tmux_template_2panes "paper"
    echo "this is the paper's tmux"
}

tmux_entertainment()
{
    tmux_template_3panes "entertainment"
    echo "this is the entertainment's tmux"
}

tmux_English()
{
    tmux_template_3panes "English"
    echo "this is the English's tmux"
}

tmux_chore()
{
    tmux_template_2panes "chore"
    echo "this is the chore's tmux"
}
#}}}

# >>>>>>>>>>>>>>>> the action function <<<<<<<<<<<<<<<<<<<<<
#{{{
action()
{
    # the variables
    # the file directories
    goal_list_dir=~/mygithub/tmux_treasure/${mode}/goal-list
    goal_all_dir=~/mygithub/tmux_treasure/${mode}/goal-all
    # the goal index
    goal_index=1
    # the actions array
    action_array=("goal" "action" "question" "task" "conclusion" "progess")
    # the path of the goal or the goal_list
    goal_path=$goal_all_dir/$3.goal # this is may be exsitent, have to check!
    goal_list_path=$goal_list_dir/goal_$3.txt # have to be specified!
    counter_path=$goal_list_dir/counter
    # get the full name of the action
    for i in ${action_array[@]}
    do
        if [ "${1:0:1}" == "${i:0:1}" ]; then
            action_full_name=$i
        fi
    done
    echo "You are doing: $action_full_name"
    case "$1" in

        "g") # goal
        #{{{
            # confirm the index 
            if [ -z "$3" ];then
                goal_path=$goal_all_dir/$goal_index.goal
            else
                goal_path=$goal_all_dir/$3.goal
            fi
            #
            case "$2" in
            # show all the goals, finished goals, not finished goals
            "show")
            #{{{
                case "$3" in
                    "a" | "y" | "n" | "d" | "w" | "m")
                    # show all the goals
                    vim $goal_list_path
                    #cat $goal_list_path
                        ;;
                    "a-" | "y-" | "n-" | "d-" | "w-" | "m-")
                    # show all the goals
                    goal_list_path=$goal_list_dir/goal_${3%%-*}.txt # delete the -
                    cat $goal_list_path
                    #vim $goal_list_path
                        ;;
                    "help")
                        echo " --------------------------------- help option ---------------------------------" 
                        echo "a/y/n/d/w/m ->: all/yes/not/day/week/month's goals, vim xxx"
                        echo
                        echo "a-/y-/n-/d-/w-/m- ->: all/yes/not/day/week/month's goals, cat xxx"
                        echo
                        echo "[1-...] ->: show the indox of the goal, for example the 88.goal"
                        echo
                        echo "help ->: give the help information"
                        echo
                        echo " -------------------------------------------------------------------------------" 
                        ;;

                    *)
                    # check if the goal index, right-> show the goal, not-> make the error
                        if [ -f $goal_path ]; then
                            vim $goal_path
                        else
                            echo "Error: the <$3> not exist! Fail to show the goal!"
                        fi
                        ;;
                esac
                ;;
            #}}}
               
            "create")
                #{{{
                # create the new goal
                ## read the counter first
                counter_line_1=$(head -1 $counter_path)
                goal_number=${counter_line_1##*:}
                ### vim open the file
                let goal_number_new=goal_number+1
                create_goal_path=$goal_all_dir/$goal_number_new.goal
                cp $goal_list_dir/goal_template.txt $create_goal_path
                sed -i "/^\*/s/\*/${goal_number_new}/g" $create_goal_path
                vim $create_goal_path
                # check the number of the goals
                goal_number_new_check=$(ls $goal_all_dir | wc -w)
                if [ ! "$goal_number_new_check" == "$goal_number_new" ];then
                    echo "The goals number may be wrong! For counter: $goal_number_new_check, but the index: $goal_number_new"
                fi
                sed -i "/^1/s/${goal_number}/${goal_number_new_check}/g" $counter_path
                ;;
                #}}}

            "help")
            #{{{
                echo " --------------------------------- help option ---------------------------------" 
                echo "show ->: show the goals"
                echo "create ->: create the goals"
                echo "help ->: show the help information"
                echo " -------------------------------------------------------------------------------" 
                ;;
            #}}}

            *)
            #{{{
            echo "Error: no command < $2 >, please use the <help> to get more information!"
                ;;
            #}}}
        esac
            ;;
        #}}}

        # show the action/quesion/task/conclusion/progress
        "a" | "q" | "t" | "c" | "p") # action
            # confirm the index
            if [ -z "$2" ];then
                goal_path=$goal_all_dir/$goal_index.goal
            else
                goal_path=$goal_all_dir/$2.goal
                echo $goal_path
            fi
            sed -n "/^>>$1/,/^<<$1/p" $goal_path # use the ">>$1 and the <<$1" as the anchor
                ;;

        # add the action/quesion/task/conclusion/progress
            "aa" | "qa" | "ta" | "ca" | "pa") # action
            echo "Append to the $action_full_name: "
            read -e append_content
            if [ -z "$2" ];then
                goal_path=$goal_all_dir/$goal_index.goal
            else
                goal_path=$goal_all_dir/$2.goal
            fi
            sed -i "/^<<${1:0:1}/i${append_content}" $goal_path # insert above the last line fo the action
            sed -n "/^>>${1:0:1}/,/^<<${1:0:1}/p" $goal_path # use the ">>$1 and the <<$1" as the anchor
                ;;

        "help") # help
            ;;

        *) #others
            ;;
    esac

    # >>>> show part <<< 
    # show all all goal

    # show the goal that has been finished

    # show the goal that has not been finished


    # show the goal about the date(today, week, month)


    # >>>> write part <<< 
    # write, renew the goal, actions, task, questions, get and so on!

    ## create the goal, have to use the number counter to count
    # " -> first creat the file, and then renew the goal_a, and the goal_n, use the counter.

    # >>> some custom part for the specifying goal <<<
    case "$mode" in
        "c++")
            ;;

        "python")
            ;;

        "vim")
            ;;

        "markdown")
            ;;

        "shell")
            ;;

        "paper")
            ;;

        "project")
            ;;
            
        "entertainment")
            ;;

        "chore")
            ;;

        "English")
            case "$1" in
                "show")
                    # show the goal, show the enhancement
                    ;;
                "write")
                    ;;
                "help")
                    ;;
            esac
            ;;

        *)
            ;;

    esac


}
#}}}

# >>>>>>>>>>>>>>>> the main script <<<<<<<<<<<<<<<<<<<<<
#{{{
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
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_c++ 
        fi
        ;;
    #}}}

    "python")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_python
        fi
        ;;
    #}}}

    "vim")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_vim
        fi
        ;;
    #}}}

    "shell")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_shell
        fi
        ;;
    #}}}

    "markdown")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_markdown
        fi
        ;;
    #}}}

    "project")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
         # create the tmux layout
            tmux_project
        fi
        ;;
    #}}}

    "paper")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_paper
        fi
        ;;
    #}}}

    "entertainment")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_entertainment
        fi
        ;;
    #}}}

    "English")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_English
        fi
        ;;
    #}}}

    "chore")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_chore
        fi
        ;;
    #}}}

    "kill-all")
    #{{{
        tmux kill-server >/dev/null 2>&1
        if [ "$?" == 0 ];then
            echo "Successfully to kill all the sessions!"
        else
            echo "No tmux sessions to kill!"
        fi
        ;;
    #}}}

    "ls")
    #{{{
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
    #}}}

    "go-love")
    #{{{
        cd $source_dir
        echo "->:${source_dir}"
        ;;
    #}}}

    "go-install")
    #{{{
        cd $install_dir
        echo "->:${install_dir}"
        ;;
    #}}}

    "love")
    #{{{
        vim $source_name
        echo "Remember to execute the < love reinstall > to make it effect!"
        ;;
    #}}}

    "reinstall"|"ri")
    #{{{
        # to install the file again to cover the original file
        love go-love
        ./install.sh && echo "----------------- reinstall successfully --------------------"
        ;;
    #}}}

    "mode")
    #{{{
        # show the mode
        echo "mode ->: $mode"
        ;;
    #}}}

    "mode-select")
    #{{{
        # select mode
        # check the $2, if the valid mode
        echo ${sessions_list[@]} | grep -qw "$2"
        if [ "$?" == 0 ]; then
            echo "mode ->: $2"
            export mode=$2
        else
            echo "Note: Invalid mode <$2>"
        fi
        ;;
    #}}}

    "action")
    #{{{
        # this is the action, the most important part
        action $2 $3 $4
        ;;
    #}}}

    "help")
    #{{{
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
        echo -e "${GREEN}: ls [all]->:${NOCOLOR} list all the existent sessions, is with the all argument, show all"
        echo
        echo -e "${GREEN}: mode->:${NOCOLOR} show the mode, for example the c++, vim..."
        echo
        echo -e "${GREEN}: help->:${NOCOLOR} show the help information"
        echo
        echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
        ;;
    #}}}
        
    *)
    #{{{
        echo "Error: No such tmux task!"
        ;;
    #}}}
esac
#}}}
