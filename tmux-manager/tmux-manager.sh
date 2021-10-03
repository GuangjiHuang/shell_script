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
    "English" \
    "type")
tmux_action="create"
source_dir=~/mygithub/shell_script/tmux-manager
source_name="${source_dir}/tmux-manager.sh"
install_dir=/opt/myscript/tmux-manager
mode_control_path=/opt/myscript/tmux-manager/mode_control.sh
# the path of the goals inside the mode
# the file directories
goal_list_dir=~/mygithub/tmux_treasure/${mode}/goal-list
goal_all_dir=~/mygithub/tmux_treasure/${mode}/goal-all
counter_path=$goal_list_dir/../counter
# get the full name of the action
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
    tmux_template_2panes "shell"
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

tmux_type()
{
    tmux_template_3panes "type"
    echo "this is the type's tmux"
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
    counter_path=$goal_list_dir/../counter
    # get the full name of the action
    for i in ${action_array[@]}
    do
        if [ "${1:0:1}" == "${i:0:1}" ]; then
            action_full_name=$i
        fi
    done
    #
    # confirm the index 
    if [ -z "$2" ];then
        goal_path=$goal_all_dir/$goal_index.goal
    else
        goal_path=$goal_all_dir/$2.goal
    fi
    # output the goals information and the action
    # --------------> Vim | Goal 12 | question; <--------------
    echo -e "--------------> ${RED}$mode${NOCOLOR} | ${GREEN}goal $goal_index${NOCOLOR} | ${CYAN}$action_full_name${NOCOLOR} <--------------"
    case "$1" in
        "gs") # goal show
        #{{{
            #
            # show all the goals, finished goals, not finished goals
            case "$2" in
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
                        echo "Error: the <$2> not exist! Fail to show the goal!"
                    fi
                    ;;
            esac
            ;;
            
        #}}}       
        "gc") # goal create 
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
        # show the action/quesion/task/conclusion/progress
        "a" | "q" | "t" | "c" | "p") # action
        #{{{
            sed -n "/^>>$1/,/^<<$1/p" $goal_path # use the ">>$1 and the <<$1" as the anchor
                ;;
        #}}}
        # add the action/quesion/task/conclusion/progress
        "aa" | "qa" | "ta" | "ca" | "pa") # action
        #{{{
            # get the number of them
            number_line=$(sed -n "/^-> .*${action_full_name}.*:.*[0-9].*$/p" $goal_path)
            x_number=${number_line##*:}
            let x_number_new=x_number+1
            # then replace the nubmer
            sed -i "/^-> .*${action_full_name}.*[0-9]\{1,3\}$/s/[0-9]\{1,3\}/${x_number_new}/g" $goal_path
            #
            echo "Add the $action_full_name: GOOD JOB ^_^"
            read -e append_content
            # if is the aa, insert the time before the append_content
            if [ "$1" == "aa" ]; then
                time_start=$(date "+%Y/%m/%d %X")
                append_content="$time_start - xxx\\n$append_content"  # add the time and the new line before the content
            fi
            # add the number oder to the append_content
            append_content="($x_number_new) $append_content"
            sed -i "/^<<${1:0:1}/i${append_content}" $goal_path # insert above the last line fo the action
            sed -n "/^>>${1:0:1}/,/^<<${1:0:1}/p" $goal_path # use the ">>$1 and the <<$1" as the anchor
            ;;
        #}}}
        "aa+") # add the time to the action
            echo "---> Done!"
            time_end=$(date "+%X")
            sed -i "s/xxx/${time_end}/g" $goal_path
            # show the finished action
            number_line=$(sed -n "/^-> .*${action_full_name}.*:.*[0-9].*$/p" $goal_path)
            x_number=${number_line##*:}
            #sed -n "/^($x_number)/,/^<<a/p" $goal_path
            end_line=$(sed -n '/^<<a/{=;p}' $goal_path)
            let end_line_number=${end_before_line%%\<*}
            let end_before_line_number=end_line_number-1
            sed -n "/([$number_line]).*:.*:.*/,${end_before_line_number}p" $goal_path
            ;;

        "help")
        #{{{
            echo " --------------------------------- help option ---------------------------------" 
            echo "gs xxx ->: show the goals"
            echo
            echo "gc ->: create the goals"
#            echo
#            echo "a/q/t/c/p/v ->: action/question/task/conclusion/progress/verbose(open the goal file)"
            echo
            echo "aa/qa/ta/ca/pa ->: add the action/question/task/concludion/progress "
            echo
            echo "help ->: show the help information"
            echo
            echo " -------------------------------------------------------------------------------" 
            ;;
        #}}}
        *)
        #{{{
            echo "Error: no command < $2 >, please use the <help> to get more information!"
            ;;
        #}}}
    esac

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

    "type")
    #{{{
        # set the environment variable
        echo "export mode=$1" > $mode_control_path
        if [ "$tmux_action" == "into" ]; then
            tmux attach -t $1
        else
            # create the tmux layout
            tmux_type
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

    "go-goal")
    #{{{
        cd ~/mygithub/tmux_treasure/${mode}
        echo "->:~/mygithuu/tmux_treasure/${mode}"
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
        path_record=$(pwd)
        clear
        love go-love
        ./install.sh && echo "----------------- reinstall successfully --------------------" && cd $path_record
        ;;
    #}}}

    "mode")
    #{{{
        # show the mode
        echo "mode ->: $mode"
        ;;
    #}}}

    "you")
    #{{{
        # show the important message, the mode, your day goal, your progress
        # the message stored in the file

        # use the sed to read the message and then show the message
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

    "renew-goal-list" | "-")
    #{{{
        # renew the goal lists, including the goals number, the goals finished and not finished
        # scan the directory and then counter them
        num_all=0
        num_finished=0
        num_not_finished=0
        # the goal_list_x path
        goal_alllist_path=$goal_list_dir/goal_a.txt
        goal_finished_path=$goal_list_dir/goal_y.txt
        goal_notfinished_path=$goal_list_dir/goal_n.txt
        # empty the file
        echo "" > $goal_alllist_path
        echo "" > $goal_finished_path
        echo "" > $goal_notfinished_path
        # loop 
        for i in $(ls $goal_all_dir | sort -h)
        do 
            # add the prefix
            i=$goal_all_dir/$i # xxx/xx.goal
            let num_all+=1
            # check the goals's progress if the xxx/xxx, only if the xxx+xxx=20, that is finished
            progress_value=$(sed -n '/^->.*progress.*/s/[^0-9/]//gp' $i) # locate the line and then delete the other character
            #echo $progress_value
            # then read the goals and store it as the string
            #goal_info=$(sed -n '1,/^$/p' $i)
            #echo -e $goal_info >> $goal_alllist_path
            sed -n '1,/^$/p' $i >> $goal_alllist_path
            #
            if [ ! "$progress_value" == "10/10" ]; then
               let num_not_finished+=1
               sed -n '1,/^$/p' $i >> $goal_notfinished_path
               #echo -e $goal_info >> $goal_notfinished_path # add the goal to the not finished
           else
               sed -n '1,/^$/p' $i >> $goal_finished_path
               #echo -e $goal_info >> $goal_finished_path # add the goal to the finished
            fi
        done
        let num_finished=num_all-num_not_finished
        echo -e "----------------- ${RED}$mode${NOCOLOR} | ${YELLOW}Goal information${NOCOLOR} -----------------"
        echo -e "(*) ${CYAN}ALL${NOCOLOR}: $num_all "
        echo
        echo -e "(+) ${GREEN}YES${NOCOLOR}: $num_finished "
        echo
        echo -e "(-) ${RED}NOT${NOCOLOR}: $num_not_finished "
        echo
        echo -e "----------------------------------------------------------"
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
        echo -e "${GREEN}: go-goal->:${NOCOLOR} go to the goal dir: ~/mygithub/tmux_treasure/..."
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
