#! /bin/bash

#check if there are the directory
dir_name=$(date "+%Y-%m")/$(date "+%m-%d")
today_path=~/mygithub/everyday_record/${dir_name}
if [ ! -d "${today_path}" ]; then
    mkdir -p ${today_path}
    echo "creat the ${today_path} successfully!"
    touch ${today_path}/plan.txt
    touch ${today_path}/learn.txt
    touch ${today_path}/question.txt
    touch ${today_path}/temp.txt
fi

# check the argumet and then open the corresponding file
case "$1" in
    "plan")
        vim ${today_path}/plan.txt
        ;;
    "learn")
        vim ${today_path}/learn.txt
        ;;
    "question")
        vim ${today_path}/question.txt
        ;;
    "temp")
        vim ${today_path}/temp.txt
        ;;
    *)
        echo -e "-> ${RED}ERROR:${NOCOLOR} Wrong input! You can just input the following option:\n-> ${GREEN}OPTION:${NOCOLOR} plan, learn, questiion, temp"
        ;;


esac
