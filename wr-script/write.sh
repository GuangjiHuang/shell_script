#! /bin/bash
#! /usr/bin/python
# define the function
func_goal()
{
    arg1=$1
    arg2=$2
    arg3=$3
    # deal with the directories
    # check if exists the ~/mygithub/goal/
    goal_dir=~/mygithub/goal/
    goal_info_path=/opt/myscript/wr-script/goal_info
    if [ ! -d "${goal_dir}" ]; then
        mkdir -p ${goal_dir}
        # copy the goal.list to the directory
        cp ${goal_info_path}/goal.list ${goal_dir}
        # mkdir that in the goal.list
        cd ${goal_dir}
        for line in $(cat "${goal_dir}/goal.list")
        do
            if [ ${line:0:2} == "./" ]; then
                mkdir ${line}
            fi
        done
    fi
    # the $2 can be the 
    # list -> list all the goal
    # the specification goal, such as the code, English, reading and so on. and then jump to it;
    case ${arg2} in 
        "list")
            # list all the goal
            echo -e "${YELLOW}------ GOALS ------${NOCOLOR}"
            for line in $(cat "${goal_dir}/goal.list")
            do
                if [ ${line:0:2} == "./" ]; then
                    line_rid_left=${line#*/}
                    line_rid_left_right=${line_rid_left%/}
                    # show the goal
                    echo -e "-> ${GREEN}${line_rid_left_right}${NOCOLOR}"
                fi
            done
            echo -e "${YELLOW}-------------------${NOCOLOR}"
            ;;
        "create")
            mkdir -p ${goal_dir}${arg3}
            # and then add the goal to the goal.list
            echo "./${arg3}/" >> "${goal_dir}/goal.list"
            echo "Create the goal: ${arg3} successfully!" 
            ;;
        "help")
            clear
            echo -e "${GREEN}------ HELP INFORMATION ------${NOCOLOR}"
            echo 
            echo -e "${GREEN}: list->${NOCOLOR}: list the all goal"
            echo 
            echo -e "${GREEN}: create->${NOCOLOR}: create the new goal"
            echo 
            echo -e "${GREEN}: help->${NOCOLOR}: show the command information"
            echo 
            echo -e "${GREEN}: xxx(the goal)->${NOCOLOR}: show the specification goal"
            ;;
        *)
            if [ -d ${goal_dir}${arg2} ]; then
                cd ${goal_dir}${arg2}
                vim ${goal_dir}${arg2}
            else
                echo -e "${RED}Warming, ${arg2} not exist!${NOCOLOR}"
                echo -e "Note: you can use the ${GREEN}< wr goal creat xxx >${NOCOLOR} to create the goal directory"
            fi
            ;;
    esac

}

copy_sth()
{
    # the var
    file_dir=/opt/myscript/wr-script/register-clipboard
    # cygwin -> /dev/clipboard; others -> /opt/myscript/wr-script/middle_station.clipboard
    if [ "$(uname -o)" == "Cygwin" ];then
        middle_station=/dev/clipboard
    else
        middle_station=/opt/myscript/wr-script/middle_station.clipboard
    fi

    if [ -z "$2" ];then
        # no the second argument, just to copy to the /dev/clipboard or the middle_station
        pwd | tr -d "\n" > $middle_station
    else
        case "$2" in
            "help")
                clear
                echo -e "${GREEN}------ wr -c help information ------${NOCOLOR}"
                echo -e "${GREEN}: (no argumetn)->${NOCOLOR}: copy the path to the middle_station."
                echo -e "${GREEN}: hlep->${NOCOLOR}: give the help information of the < wr -v xxx>."
                echo -e "${GREEN}: [a-z] ->${NOCOLOR}: copy the path to the register [a-z]."
                echo -e "${GREEN}: [a-z] - ->${NOCOLOR}: copy, register[a-z] -> middle_station."
                echo -e "${GREEN}: [a-z] [a-z] ->${NOCOLOR}: copy, register[a-z] -> register[a-z]."
                ;;

                *)
                file_path=$file_dir/$2 # this is the file name
                if [ -f $file_path ]; then
                    # if the $3 exist, the register -> to the middle station
                    if [ -z "$3" ]; then
                        pwd | tr -d "\n" > $file_path # exist
                    elif [ "$3" == "-" ];then
                        # a...z -> middle_station, for example: a -> /dev/clipboard
                        cat $file_path > $middle_station
                    else
                        # copy to the other register <a-z>
                        # need to check if your input is the register
                        to_file_path=$file_dir/$3
                        if [ -f $to_file_path ]; then
                            cat $file_path > $to_file_path # exist
                        else
                            echo "Note: for the argument 3 you should input \"-\"or the <a-z>, instead of the $3!"
                        fi
                    fi
                    # and then renew the whole_path_information.list
                else
                    echo "\$2 must be the <a-z>, your input $2 is wrong!"
                fi
                ;;
        esac
    fi
}

past_sth()
{
    # the var
    file_dir=/opt/myscript/wr-script/register-clipboard
    # cygwin -> /dev/clipboard; others -> /opt/myscript/wr-script/middle_station.clipboard
    if [ "$(uname -o)" == "Cygwin" ];then
        middle_station=/dev/clipboard
    else
        middle_station=/opt/myscript/wr-script/middle_station.clipboard
    fi

    if [ -z "$2" ]; then
        # no the second argument, show the middle_sation
        cat $middle_station
        echo # output the new line
    else
        case "$2" in
            "go")
                cd $(cat $middle_station)
                echo "->$(pwd)"
                ;;
            "list")
                # loop and then show all the file and the file's content
                for i in $file_dir/*
                do
                    content_path=$(cat $i)
                    # if the $3 is the all, then show all the register even it is empty
                    if [ -z "$content_path" ]  && [ "$3" != "all" ];then
                            continue
                    else
                        echo "${i##*/}: $content_path"
                    fi
                done
                ;;
            "help")
                clear
                echo -e "${GREEN}------ wr -v help information ------${NOCOLOR}"
                echo 
                echo -e "${GREEN}: (no argument)->${NOCOLOR}: show the middle_station's path."
                echo 
                echo -e "${GREEN}: list->${NOCOLOR}: list all the regsiter content, reg<a-z>."
                echo 
                echo -e "${GREEN}: go->${NOCOLOR}: cd to the middle_station, for example cd cat$(/dev/clipboard)."
                echo 
                echo -e "${GREEN}: hlep->${NOCOLOR}: give the help information of the < wr -v xxx>."
                echo 
                echo -e "${GREEN}: [a-z] ->${NOCOLOR}: show the [a-z] register's content."
                echo 
                echo -e "${GREEN}: [a-z] go ->${NOCOLOR}: cd to the register [a-z] path."
                ;;

                *)
                # check the file path
                file_path=$file_dir/$2
                if [ -f $file_path ]; then
                    # do something
                    if [ -z "$3" ]; then
                        # just show the path
                        echo "${file_path##*/}: $(cat $file_path)"
                    elif [ "$3" == "go" ];then
                        cd $(cat $file_path)
                        echo "->$file_path"
                    fi
                else
                    echo "\$2 must be the <a-z>, your inpout $2 is wrong!"
                fi
                ;;
        esac
    fi
}

#check if there are the directory
dir_name=$(date "+%Y-%m")/$(date "+%m-%d")
# when you use the command: wr, it will creat the environment varialbe that records the path 
# of the today_record path, so you can easily to open it inside the vim using the :e $p_today
export p_today=~/mygithub/everyday_record/${dir_name}/
#if you want to use the date-pointer's path, you have to specify the -p as the $2
if [ "$2" == "-p" ]; then
    dir_name=$(cat /opt/myscript/wr-script/date-pointer.txt)
    # check if $dir_name is empty
    if [ -z ${dir_name} ]; then
        echo -e "->${RDD}ERROR:${NOCOLOR}the dir_name is empty! Please key ctrl+c to stop!"
        read
    fi
elif [ "$2" == "-z" ]; then
    dir_name="compress"
fi
today_path=~/mygithub/everyday_record/${dir_name}
if [ ! -d "${today_path}" ]; then
    mkdir -p ${today_path}
    echo "creat the ${today_path} successfully!"

    if [ "$2" == "-z" ]; then
        touch ${today_path}/learn_a.txt 
        touch ${today_path}/question_a.txt
        touch ${today_path}/review_a.txt
    else
        separate_sign="=================================="
        touch ${today_path}/plan.txt && echo -e  "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/plan.txt
        touch ${today_path}/learn.txt && echo -e "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/learn.txt
        touch ${today_path}/question.txt && echo -e "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/question.txt
        touch ${today_path}/temp.txt && echo -e "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/temp.txt
        touch ${today_path}/type.pratice && echo -e "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/type.pratice
        # add the todolist.txt (copy the template to it );
        separate_sign_2="--------------------------------------"
        touch ${today_path}/todolist.txt && echo -e "${separate_sign_2}\n------ $(datef) -------\n${separate_sign_2}" > ${today_path}/todolist.txt
        cat /opt/myscript/wr-script/todolist/todolist.template >> ${today_path}/todolist.txt
        echo "${today_path}/todolist.txt" > /opt/myscript/wr-script/todolist/date-path.txt
        # copy the type.template to the type.pratice
        cat /opt/myscript/wr-script/type.template >> ${today_path}/type.pratice
        echo $separate_sign_2 >> ${today_path}/type.pratice
    fi

fi

# check the argumet and then open the corresponding file
case "$1" in
    "love")
        . /opt/myscript/wr-script/my_sq/my_sp.sh
        ;;
    "plan")
        vim ${today_path}/plan.txt
        ;;
    "learn")
        if [ "$2" == "-z" ]; then
            vim ${today_path}/learn_a.txt
        else
            vim ${today_path}/learn.txt
        fi
        ;;
    "question")
        if [ "$2" == "-z" ]; then
            vim ${today_path}/question_a.txt
        else
            vim ${today_path}/question.txt
        fi
        ;;
    "temp")
        vim ${today_path}/temp.txt
        ;;
    "review")
        if [ "$2" == "-z" ]; then
            vim ${today_path}/review_a.txt
        fi
        ;;
    "pointer-check")
        echo -e "Point to -> $(cat /opt/myscript/wr-script/date-pointer.txt)"
        ;;
    "pointer")
        dir_name=$2-$3/$3-$4
        pointer_path=~/mygithub/everyday_record/${dir_name}
        # check if the path exists?
        if [ -d "${pointer_path}" ]; then
            echo ${dir_name} > /opt/myscript/wr-script/date-pointer.txt
            echo -e "->Successfully points to the ${dir_name}"
        else
            echo -e "->${RED}ERROR${NOCOLOR}: ${dir_name} not exist! Fail!"
        fi
        ;;
    "go-e")
        # go to the everyday_record path
        #e_dest=~/mygithub/everyday_record/compress/
        e_dest=${today_path}/
        cd $e_dest
        echo "->: $e_dest"
        ;;
    "go-wr")
        # go to the everyday_record path
        wr_dest=~/mygithub/shell_script/wr-script/
        cd $wr_dest
        echo "->: $wr_dest"
        ;;
    "go-install")
        # go to the /opt/myscript/wr-script/
        install_dest=/opt/myscript/wr-script/
        cd $install_dest
        echo "->: $install_dest"
        ;;
    "go-goal")
        # go to the /mygithub/goal/
        goal_dst=~/mygithub/goal/
        cd ${goal_dst}
        echo "->: ${goal_dst}"
        ;;
    "merge")
        cat ${today_path}/question.txt >> ~/mygithub/everyday_record/compress/question_a.txt
        cat ${today_path}/learn.txt >> ~/mygithub/everyday_record/compress/learn_a.txt
        echo "Good job! Has been merging xxx.txt to xxx_a.txt! Successfully!"
        ;;
    "all")
        vim ~/mygithub/everyday_record/compress/
        ;;
    "rest")
        . /opt/myscript/wr-script/my_sq/rest.sh
        ;;
    "moyu")
        . /opt/myscript/wr-script/my_sq/moyu.sh
        ;;
    "xiaban")
        wr merge
        cd ~/mygithub/everyday_record/
        . ~/mygithub/everyday_record/xiaban.sh
        ;;
    "todo")
        vim ${today_path}/todolist.txt
        ;;
    "todo-cfg")
        cd /opt/myscript/wr-script/todolist/
        vim .
        ;;
    "todo-exe")
        cd /opt/myscript/wr-script/todolist/
        ./r_c_w.exe && echo ">>>>>> Execute the program successfully! <<<<<<"
        cd -
        wr todo
        ;;
    "wr")
        vim ~/mygithub/shell_script/wr-script/write.sh
        ;;
    "goal")
        func_goal $1 $2 $3
        ;;
    "type")
        if [ -z $2 ]; then
             #open the type.pratice
             vim ${today_path}/type.pratice
        elif [ "$2" == "m" ]; then
             #open the type.template
             vim /opt/myscript/wr-script/type.template
        elif [ "$2" == "c" ]; then
             # copy the /opt/myscirpt/wr-script/type.template to the ~/mygithub/shell_script/wr-script/type.tempate
             cp /opt/myscript/wr-script/type.template ~/mygithub/shell_script/wr-script/type.template
             echo ":->Successfully renew the type.template!"
         elif [ "$2" == "renew" ]; then
             # copy the type.template to the type.pratice, because the type.tempate has been renew
             echo -e "${separate_sign}\n$(datef)\n${separate_sign}\n" > ${today_path}/type.pratice
             cat /opt/myscript/wr-script/type.template >> ${today_path}/type.pratice
             echo $separate_sign_2 >> ${today_path}/type.pratice
             echo ":->Has been renew the ${today_path}/type.pratice"
             # if you want to open it, add the $3
             if [ "$3" == "o" ]; then
                 vim ${today_path}/type.pratice
             fi
        else
             echo -e "Error: no the command: \" wr type $2 \""
        fi
        ;;
    "pretend")
        # execute the program
        /opt/myscript/wr-script/c++_interesting_program/pretend_to_do_something.exe $2 $3 $4
        ;;

    "reinstall")
        # to install the file again to cover the original file
        wr go-wr
        ./install.sh && echo "----------------- reinstall successfully --------------------"
        ;;

    "-")
        # go to the important place that you want
        vim $HOME/mygithub/goal/paper/determine_title_report.ddl
        ;;
    "-c")

        # pwd | tr -d "\n" > /dev/clipboard
        # call  the copy_sth()
        copy_sth $1 $2 $3
        ;;
    "-v")
        # >>>>>>>>>>>>>  obsolete <<<<<<<<<<<<<<<<<<<<<<
        ## if the $3 exist and equal to the "go", then jump to the path
        #if [ "$3" == "go" ];then
        #    cd $(cat /dev/clipboard)
        #    echo "->:$(pwd)"
        #else
        #    cat /dev/clipboard
        #    echo  # output the new line 
        #fi
        # >>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<
        # call the past_sth() 
        past_sth $1 $2 $3
        ;;
    "preview_md_exe")
        # >>>>>>> the window's when-change.exe <<<<<<<<
        # the variables and the arguments
        typora_dir=/cygdrive/d/Typora
        when_changed_option=$3
        document_name=$2
        document_path=$(pwd)/$2
        
        # check the $2 argument
        echo "the document's name is the: ${document_name}" 
        if [ -n "${document_name}" ]; then
            # the command
            cd $typora_dir # to the directory first
            # for the window, you can not use the ./ as the prefix
            when-changed.exe ${when_changed_option} "${document_path}" Typora.exe "${document_path}"
        else
            echo "Lack the document, fail to run!"
        fi
        echo "Finish to run!"
        ;;

    "preview_md")
        # >>>>>>>>>> fail to execute: 
        # N) no matter what the when-changed is(the window or the unix), it will use the window's
        # python(python3.7 not the python3.8);
        # <<<<<<<<< 

        # >>>>>>> the cygwin's when-change <<<<<<<<
        # the variables and the arguments
        typora_path=/cygdrive/d/Typora/Typora.exe
        when_changed_option=$3
        document_name=$2
        # cygwin_path -> window path
        document_path_cygwin=$(pwd)/$2
        document_path_window=$(cygpath -w "$(pwd)/$2")
        
        # check the $2 argument
        echo "the document's name is the: ${document_path_cygwin}" 
        if [ -n "${document_name}" ]; then
            # the command
            #cd $typora_dir # to the directory first
            # in the unix, just to use the full path is ok to execute the typora
            # the typora is the window's program, it just recognize the window's path
            /usr/local/bin/when-changed ${when_changed_option} "${document_path_cygwin}" ${typora_path} "${document_path_window}" 
            #/usr/local/bin/when-changed ${when_changed_option} "${document_path_cygwin}" echo "execute"
        else
            echo "Lack the document, fail to run!"
        fi
        echo "Finish to run!"
        ;;

    "fish")
        # execute the asciiquarium
        if [ "$(uname -o)" == "Cygwin" ];then
            echo "------ No Fish --------"
        else
            asciiquarium
        fi
        ;;

    "help")
        clear
        echo -e "${YELLOW}-------------------------------> OPTION <---------------------------------------${NOCOLOR}"
        #echo -e "--------------------${GREEN}OPTION${NOCOLOR}--------------------"

        echo -e "${GREEN}: -->:${NOCOLOR} go to the direct place!"
        echo
        echo -e "${GREEN}: all->:${NOCOLOR} use .../compress/, you can edit the compress's file"
        echo
        echo -e "${GREEN}: fish->:${NOCOLOR} execute the command < asciiquarium > in the linux system"
        echo
        echo -e "${GREEN}: go-e->${NOCOLOR}: cd to the everyday_record"
        echo 
        echo -e "${GREEN}: go-wr->${NOCOLOR}: cd to the write-scritp.sh"
        echo 
        echo -e "${GREEN}: go-goal->${NOCOLOR}: cd to the goal directroy"
        echo 
        echo -e "${GREEN}: goal xxx.txt${NOCOLOR}: about the goal, you can use the command < wr goal help > for more information"
        echo 
        echo -e "${GREEN}: help->${NOCOLOR}: get the help"
        echo 
        echo -e "${GREEN}: learn->${NOCOLOR}: go to the learn.txt"
        echo 
        echo -e "${GREEN}: love->${NOCOLOR}: my dear sq"
        echo 
        echo -e "${GREEN}: merge->${NOCOLOR}: merge the today's question, plan to the compress's directory"
        echo 
        echo -e "${GREEN}: moyu->${NOCOLOR}: show the moyu as the screen"
        echo 
        echo -e "${GREEN}: pointer-check->${NOCOLOR}: to show the pointer"
        echo 
        echo -e "${GREEN}: pointer->${NOCOLOR}: year-month-day-> to set the pointer"
        echo 
        echo -e "${GREEN}: pretend->${NOCOLOR}: run the fake program in the terminal to pretend to do something!"
        echo 
        echo -e "${GREEN}: preview_md->${NOCOLOR}:(the cygwin mode)use the when-changed command, when you write the markdwon in the vim, save it, show in the typora immediately"
        echo 
        echo -e "${GREEN}: preview_md_exe->${NOCOLOR}:(the window mode)use the when-changed command, when you write the markdwon in the vim, save it, show in the typora immediately"
        echo 
        echo -e "${GREEN}: question->${NOCOLOR}: go the question.txt"
        echo 
        echo -e "${GREEN}: reinstall->${NOCOLOR}: reinstall the file, to cover the original file"
        echo 
        echo -e "${GREEN}: rest->${NOCOLOR}: show the rest as the screen"
        echo 
        echo -e "${GREEN}: temp->${NOCOLOR}: go the temp.txt"
        echo 
        echo -e "${GREEN}: todo->${NOCOLOR}: edit the todolist.txt"
        echo 
        echo -e "${GREEN}: todo-cfg->${NOCOLOR}: modify the todo configuration"
        echo 
        echo -e "${GREEN}: todo-exe->${NOCOLOR}: execute the prgrom to read, count, and then write"
        echo 
        echo -e -e "${GREEN}: type x y->${NOCOLOR}: 1) no x: open the type.pratice; 2) x=m: change the type.template manually;\n"\
        "             3) x=c: copy the /opt/.../type.template to the ~/mygithub/.../type.template;\n"\
        "             4)x=renew: renew the today's type.pratice, if the y=o, it will open the type.pratice"
        echo 
        echo -e "${GREEN}: xiaban->${NOCOLOR}: Upload the file to the github reposity, and then off duty"
        echo 
        echo -e "${GREEN}: wr->${NOCOLOR}:modify the write.sh"
        echo 
        echo -e "${GREEN}: -c->${NOCOLOR}:copy something! Type <wr -c help> for more information!"
        echo 
        echo -e "${GREEN}: -v->${NOCOLOR}:past something! Type <wr -v help> for more information!"
        echo 
        echo -e "${YELLOW}--------------------------------------------------------------------------------${NOCOLOR}"
        ;;
    *)
        echo -e "-> ${RED}ERROR:${NOCOLOR} Wrong input! You can just input the following option:\n-> ${GREEN}OPTION:${NOCOLOR} love, plan, learn, question, temp, help, pointer %Y-%m-%d, pointer-check, go-e, go-wr, review, all, rest, todo, todo-cfg, todo-exe, type x... and so on "
        echo -e "${GREEN}You can use the wr help for more information!${NOCOLOR}"
        ;;
esac
