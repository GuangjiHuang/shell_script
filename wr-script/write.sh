#! /bin/bash

#check if there are the directory
dir_name=$(date "+%Y-%m")/$(date "+%m-%d")

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
        touch ${today_path}/plan.txt && echo $(datef) > ${today_path}/plan.txt
        touch ${today_path}/learn.txt && echo $(datef) > ${today_path}/learn.txt
        touch ${today_path}/question.txt && echo $(datef) > ${today_path}/question.txt
        touch ${today_path}/temp.txt && echo $(datef) > ${today_path}/temp.txt
    fi

fi

# check the argumet and then open the corresponding file
case "$1" in
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
    "help")
        echo -e "->${GREEN}OPTION:${NOCOLOR}"
        echo ": learn-> go to the learn.txt"
        echo ": temp-> go the temp.txt"
        echo ": question-> go the question.txt"
        echo ": help-> get the help"
        echo ": pointer year-month-day-> to set the pointer"
        echo ": pointer-check-> to show the pointer"
        ;;
    "go-e")
        # go to the everyday_record path
        e_dest=~/mygithub/everyday_record/
        cd $e_dest
        echo "->: $e_dest"
        ;;
    "go-wr")
        # go to the everyday_record path
        wr_dest=~/mygithub/shell_script/wr-script/
        cd $wr_dest
        echo "->: $wr_dest"
        ;;
    *)
        echo -e "-> ${RED}ERROR:${NOCOLOR} Wrong input! You can just input the following option:\n-> ${GREEN}OPTION:${NOCOLOR} plan, learn, question, temp, help, pointer %Y-%m-%d, pointer-check, go-e, go-wr, review"
        ;;


esac
