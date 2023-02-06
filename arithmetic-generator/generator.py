import random
import os
import time
import re
import argparse

# define the color
RED = "\033[44;31m"
BLUE = "\033[0;45m"
NOCOLOR = "\033[0m"

def concealAnswer(content, is_use_color=True):
    conceal_content = ""
    flag = False
    for i in range(len(content)):
        ch = content[i]
        if ch == "=":
            flag = True
            conceal_content += ch
        else:
            if flag and ch.isdigit():
                conceal_content += " "
                # cancle the flag
                if i+1<len(content) and not content[i+1].isdigit():
                    flag = False
            else:
                conceal_content += ch
    # change the content to add the color
    if not is_use_color:
        return conceal_content
    return showColor(conceal_content)

def showColor(conceal_content, color=BLUE):
    new_conceal_content = ""
    c_len = len(conceal_content)
    k = 0
    while k < c_len:
        ch = conceal_content[k]
        if ch == "(":
            # not the digit
            while not conceal_content[k].isdigit():
                new_conceal_content += conceal_content[k]
                k += 1
            # inside the digit 
            number_id = ""
            while conceal_content[k].isdigit():
                number_id += conceal_content[k]
                k += 1
            if int(number_id) % 10 < 2:
                new_conceal_content += color + number_id + NOCOLOR
            else:
                new_conceal_content += number_id
            #while conceal_content[k] != ")":
            #    if conceal_content[k].isdigit() and not conceal_content[k-1].isdigit():
            #        new_conceal_content += color
            #        new_conceal_content += conceal_content[k]
            #    elif conceal_content[k].isdigit() and not conceal_content[k+1].isdigit():
            #        new_conceal_content += conceal_content[k]
            #        new_conceal_content += NOCOLOR
            #    else:
            #        new_conceal_content += conceal_content[k]
            #    k += 1
        else:
            new_conceal_content += ch
            k += 1
    return new_conceal_content
#def genEqual(is_mul=True, show_answer=False):
#    is_write = True
#    cal_type_ls = ["+", "-", "x", "÷"];
#    # setting 
#    mul_min, mul_max = 2, 99
#    gen_num = 100
#    if is_mul:
#        # the min and the max
#        add_min, add_max = 100, 1000
#    else:
#        add_min, add_max = 1, 100
#
#    equal_list = []
#    i = 0
#    while i < gen_num:
#        equal_str = ""
#        # deal with the add num and the mul num
#        add_num1 = random.randint(add_min, add_max)
#        add_num2 = random.randint(add_min, add_max)
#        if add_num2 > add_num1:
#            add_num1, add_num2 = add_num2, add_num1
#        mul_num1 = random.randint(mul_min, mul_max)
#        mul_num2 = random.randint(mul_min, mul_max)
#        #
#        cal_type_id = random.randint(0,len(cal_type_ls)-1)
#        cal_type_str = cal_type_ls[cal_type_id]
#        cal_type = cal_type_str
#        #
#        if cal_type_str == "x":
#            if not is_mul:
#                continue
#            cal_type = "*"
#            num1, num2 = mul_num1, mul_num2
#        elif cal_type_str == "÷":
#            if not is_mul:
#                continue
#            cal_type = "/"
#            num1, num2 = mul_num1, mul_num2
#            if num1 % num2 != 0:
#                continue
#        else:
#            num1, num2 = add_num1, add_num2
#        #
#        result = int(eval(f"{num1} {cal_type} {num2}"))
#        equal_str = f"{num1:^6} {cal_type_str} {num2:^6} = {result}"
#        equal_list.append(equal_str)
#        # renew the i
#        i += 1
#    file_name = time.strftime("%Y-%m-%d")
#    # the content 
#    student_name = "周诗雅" if is_mul else "周佳燕"
#    content = f"\ndate: {file_name}{' '*12}姓名:{student_name}{' '*12}分数:\n"
#    content += "-" * 80
#    for k, ele in enumerate(equal_list):
#        if k % 2 == 0:
#            content += f"\n({k+1:^4}) "
#        else:
#            space_str = " " * 3
#            content += f"{space_str}({k+1:^4})"
#        #content += f"{ele:40}"
#        content += f"{ele}"
#    content += "\n" + "-" * 80
#    # show the result
#    file_path = os.path.join("data", time.strftime(f"%Y-%m-%d_{student_name}")+".txt")
#    if is_write:
#        with open(file_path, "w") as f:
#            f.write(content)
#    os.system("clear")
#    if show_answer:
#        print(content)
#    else:
#        print(concealAnswer(content))

def printFile(file_path, is_show_answer, color):
    with open(file_path, "r") as f:
        content = f.read()
    if is_show_answer:
        #print(content)
        print(showColor(content, color))
    else:
        print(concealAnswer(content), color)

def add(num_range1, num_range2):
    # generate the num1 and num2
    num1 = random.randint(num_range1, num_range2)
    num2 = random.randint(num_range1, num_range2)
    num1 = num1 - (num1 % 10)
    num2 = num2 - (num2 % 10)
    # adjust the num1 and the num2
    mod1 = random.randint(2, 9)
    mod2 = random.randint(11-mod1, 9)
    num1, num2 = num1+mod1, num2+mod2
    # generate the num2
    result = int(eval(f"{num1} + {num2}"))
    equal_str = f"{num1:^6} + {num2:^6} = {result}"
    return equal_str 

def sub(num_range1, num_range2):
    # generate the num1 and num2
    num1 = random.randint(num_range1, num_range2)
    num2 = random.randint(num_range1, num_range2)
    num1 = num1 - (num1 % 10)
    num2 = num2 - (num2 % 10)
    num1, num2 = (num2, num1) if num2 > num1 else (num1, num2)
    if num2 == 90:
        num1 -= 10
        num2 -= 10
    if num1 == num2:
        num1 += 10
    if num1 == num2 + 10:
        if num1 // 10 == 9:
            num2 -= 10
        else:
            num1 += 10
    # adjust the num1 and the num2
    percentage_number = random.randint(0,9)
    if percentage_number >= 4:
        mod1 = random.randint(0, 8)
        mod2 = random.randint(mod1+1, 9)
    else:
        mod1 = random.randint(0,9)
        mod2 = random.randint(0,9)
    num1, num2 = num1+mod1, num2+mod2
    # generate the num2
    result = int(eval(f"{num1} - {num2}"))
    equal_str = f"{num1:^6} - {num2:^6} = {result}"
    return equal_str 

def mul(num_range1, num_range2):
    # generate the num1 and num2
    num1 = random.randint(num_range1, num_range2)
    num2 = random.randint(num_range1, num_range2)
    num1 = num1 - (num1 % 10)
    num2 = num2 - (num2 % 10)
    # adjust the num1 and the num2
    mod1 = random.randint(5, 9)
    mod2 = random.randint(5, 9)
    num1, num2 = num1+mod1, num2+mod2
    # generate the num2
    result = int(eval(f"{num1} * {num2}"))
    equal_str = f"{num1:^6} x {num2:^6} = {result}"
    return equal_str 

def divide(num_range1, num_range2):
    # not use the num_range1 and the num_range2
    # xxxx / xx
    num1 = random.randint(1001, 9999)
    num2 = random.randint(11, 99)
    ret_div = num1 // num2
    num1 = ret_div * num2
    result = int(eval(f"{num1} / {num2}"))
    equal_str = f"{num1:^6} ÷ {num2:^6} = {result}"
    return equal_str 

def generateEqual(cal_type, num, num_range1, num_range2):
    ret_ls = []
    cal_func = None
    if cal_type == "+":
        cal_func = add
    elif cal_type == "-":
        cal_func = sub
    elif cal_type == "x":
        cal_func = mul
    elif cal_type == "÷":
        cal_func = divide
    #
    for i in range(num):
        ret_ls.append(cal_func(num_range1, num_range2))
    return ret_ls

def writeFile(data_dir, is_sister, show_answer):
    # {"x": [11, 99, 0], "+" .... }
    total_number = 100
    #cfg = {"x": [11, 999, 0], "+": [1001, 9999, 5], "-": [1001, 9999, 5]} if is_sister else \
    cfg = {"x": [11, 999, 0]} if is_sister else \
            {"-": [11, 999, 0], "+": [11, 999, 0]} 
    #cfg = {"x": [11, 99, 0],} if is_sister else \
    #        {"-": [11, 99, 0], "+": [11, 99, 0]} 
    equal_list = []
    # deal with the num of the cal type
    sum_specify = sum([i[2] for i in cfg.values()])
    default_number = total_number - sum_specify
    default_number = 0 if default_number < 0 else default_number
    average_len = 0
    for i in cfg.values():
        if i[2] == 0:
            average_len += 1
    default_number = default_number // average_len
    # loop
    for cal_type, ls in cfg.items():
        num_gen = ls[2] if ls[2] else default_number
        equal_list += generateEqual(cal_type, num_gen, ls[0], ls[1])

    # shuffle the equal_list
    random.shuffle(equal_list)
    file_name = time.strftime("%Y-%m-%d")
    # the content 
    student_name = "周诗雅" if is_sister else "周佳燕"
    content = f"日期: {file_name}{' '*12}姓名:{student_name}{' '*12}分数:\n"
    content += "-" * 65
    for k, ele in enumerate(equal_list):
        if k % 2 == 0:
            if k == 0:
                content += f"\n({k+1:^4}) "
            else:
                content += f"\n\n({k+1:^4}) "
        else:
            space_str = " " * 6
            content += f"{space_str}({k+1:^4})"
        #content += f"{ele:40}"
        content += f"{ele:26}"
    content += "\n" + "-" * 65
    # show the result
    file_path_no_answer = os.path.join(data_dir, "answer", time.strftime(f"%Y-%m-%d_{student_name}")+".txt")
    file_path_with_answer = os.path.join(data_dir, "noanswer", time.strftime(f"%Y-%m-%d_{student_name}")+".txt")
    is_write = True
    if is_write:
        with open(file_path_no_answer, "w") as f:
            f.write(content)
        with open(file_path_with_answer, "w") as f:
            f.write(concealAnswer(content, False))
    os.system("clear")
    if show_answer:
        print(content)
    else:
        print(concealAnswer(content))

# the main
if __name__ == "__main__":
    # the random
    lt = time.localtime()
    RAND_NUM = lt.tm_year * lt.tm_mon * lt.tm_mon * lt.tm_mday
    random.seed(RAND_NUM)
    #random.seed(9)
    # deal with the arguments
    # name, answer or not
    parser = argparse.ArgumentParser()
    parser.add_argument("name_type", type=str)
    parser.add_argument("show_answer", type=int)
    parser.add_argument("--date", type=str)
    parser.add_argument("--color", type=str)
    parser.add_argument("--today", action="store_true") # to move the today's document to the desktop
    args = parser.parse_args()
    #
    name_type = args.name_type
    show_answer = args.show_answer
    # deal with the date
    if args.date:
        date_str = args.date
    else:
        date_str = time.strftime("%Y-%m-%d")
    # is_move_to_today
    is_move_to_today = args.today
    # get the dir and check
    s_year, s_month, s_day = date_str.split("-")
    data_dir = f"data/{s_year}/{s_month}/{s_day}/"
    data_dir_answer = os.path.join(data_dir, "answer")
    data_dir_noanswer = os.path.join(data_dir, "noanswer")
    if not os.path.exists(data_dir_answer):
        os.makedirs(data_dir_answer)
    if not os.path.exists(data_dir_noanswer):
        os.makedirs(data_dir_noanswer)
    # deal with the color 
    if args.color:
        color_str = f"\033[0;{args.color}m" 
    else:
        color_str = BLUE
    file_path_1 = os.path.join(data_dir, "answer", date_str+"_周诗雅.txt")
    file_path_2 = os.path.join(data_dir, "answer", date_str+"_周佳燕.txt")
    # generate the file 
    #if not os.path.exists(file_path_1):
    #    #genEqual(is_mul=True, show_answer=True)
    #    writeFile(is_sister=True, show_answer=True)
    #if not os.path.exists(file_path_2):
    #    #genEqual(is_mul=False, show_answer=True)
    #    writeFile(is_sister=False, show_answer=True)
    writeFile(data_dir, is_sister=True, show_answer=True)
    writeFile(data_dir, is_sister=False, show_answer=True)
    # show the file
    #print("color is the: ", color_str)
    if name_type == "g":
        os.system("clear")
        printFile(file_path_1, show_answer, color_str)
    elif name_type == "l":
        os.system("clear")
        printFile(file_path_2, show_answer, color_str)
    else:
        print("[Error]: nametype must be the: g or l! For example: python xxx.py 'g' 1")

    # to move to today
    if is_move_to_today:
        file_dir_answer = os.path.join(data_dir, "answer")
        file_dir_noanswer = os.path.join(data_dir, "noanswer")
        save_dir = "../今日算术题"
        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
        os.system(f"cp -r {file_dir_answer} {save_dir}")
        os.system(f"cp -r {file_dir_noanswer} {save_dir}")
