#! /bin/bash

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
        e_dest=~/mygithub/everyday_record/compress/
        cd $e_dest
        echo "->: $e_dest"
        ;;
    "go-wr")
        # go to the everyday_record path
        wr_dest=~/mygithub/shell_script/wr-script/
        cd $wr_dest
        echo "->: $wr_dest"
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
    "help")
        echo -e "->${GREEN}OPTION:${NOCOLOR}"
        echo ": love->: my dear sq"
        echo ": learn->: go to the learn.txt"
        echo ": temp->: go the temp.txt"
        echo ": question->: go the question.txt"
        echo ": help->: get the help"
        echo ": pointer->: year-month-day-> to set the pointer"
        echo ": pointer-check->: to show the pointer"
        echo ": go-e->: cd to the everyday_record"
        echo ": go-wr->: cd to the write-scritp.sh"
        echo ": merge->: merge the today's question, plan to the compress's directory"
        echo ": all->: use .../compress/, you can edit the compress's file"
        echo ": rest->: show the rest as the screen"
        echo ": moyu->: show the moyu as the screen"
        echo ": xiaban->: off duty"
        ;;
    *)
        echo -e "-> ${RED}ERROR:${NOCOLOR} Wrong input! You can just input the following option:\n-> ${GREEN}OPTION:${NOCOLOR} love, plan, learn, question, temp, help, pointer %Y-%m-%d, pointer-check, go-e, go-wr, review, all, rest"
        ;;
esac
