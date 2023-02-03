import time
import re
import os

# about the color   
# the color
no_color = "\033[0m"
dark = "\033[0;30m"
light_dark = "\033[1;30m"
red = "\033[0;31m"
light_red = "\033[1;31m"
green = "\033[0;32m"
light_green = "\033[1;32m"
orange = "\033[0;33m"
yellow = "\033[1;33m"
blue = "\033[0;34m"
light_blue = "\033[1;34m"
purple = "\033[0;35m"
light_purple = "\033[1;35m"
cayon = "\033[0;36m"
light_cayon = "\033[1;36m"
light_gray = "\033[0;37m"
white = "\033[1;37m"

def color(color_, content):
    return color_ + content + no_color

def get_record_score(record_path):
    #{{{
    effective_time = 0
    all_time = 0
    #
    with open(record_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    # append the block to the list
    blocks_ls = []
    block = ""
    for line in lines:
        if line.strip(" ") == "\n":
            if "mins" in block:
                blocks_ls.append(block)
            # clear the block
            block = ""
        else:
            block += line
    if "mins" in block:
        blocks_ls.append(block)
    # 
    for block in blocks_ls:
        # get duration time
        time_duration_ls = [line for line in block.split("\n") if line.strip()]
        if len(time_duration_ls) == 3:
            time_duration_line = time_duration_ls[0]
        else:
            time_duration_line = time_duration_ls[1]
        time_duration = int(time_duration_line.split()[1])
        # get percent
        percent = 60
        if "%" in block:
            idx = block.index("%")
            i = idx - 1 # move back to the digit
            while i>=0 and block[i].isdigit():
                i -= 1
            percent = int(block[i:idx])
            if percent > 100:
                percent = 100
        # 
        #print(f"duration: {time_duration}, percent: {percent}")
        # renew the all_time and the effective time
        all_time += time_duration
        effective_time += int(percent * time_duration / 100)
    
    # get the score
    scores = int(effective_time / all_time * 100)
    #print(f"-> record: {scores} points")
    return scores# }}}

def get_arragement_score(arragement_path, full_score_all_time=45*9): # 9 tasks every day 
    # work time and all time{{{
    work_time = 0
    all_time = 0
    #
    with open(arragement_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    for line in lines:
        line = line.strip()
        if line.startswith("t") or line.startswith("[Y]"):
            tmp_time = 0
            if "#" not in line:
                tmp_time = 45 # minutes
            else:
                # specify the duration time
                time_duration_part = line.split("#")[-1].strip()
                for ch in time_duration_part:
                    if ch.isdigit():
                        tmp_time = tmp_time * 10 + int(ch)
                    else:
                        break
                if not tmp_time:
                    tmp_time = 45 # set the default 
            # renew the work_time and all_time
            #print(tmp_time)
            all_time += tmp_time
            if line.startswith("[Y]"):
                work_time += tmp_time
    # count
    #print(f"work_time: {work_time}, all time: {all_time}")
    if all_time < full_score_all_time:
        all_time = full_score_all_time
    scores = int(work_time / all_time * 100) 
    #print(f"-> arragemetn: {scores} points")
    return scores# }}}

def get_line_number(line):
    for i in range(len(line)):
        if line[i].isdigit():
            break
    # now i is the digit or the end of the line
    l_num = 0
    while line[i].isdigit() and i<len(line):
        l_num = l_num * 10  + int(line[i])
        i += 1
    return str(l_num)

def get_todolist_score(todolist_path):
    with open(todolist_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    # get the x, and the y, and the score
    x_ls = list()
    y_ls = list()
    scores = 0
    #
    x_line_number = -1
    y_line_number = -1
    score_line_number = -1
    for l, line in enumerate(lines):
        line = line.strip()
        if line.startswith("(x)"):
            x_ls.append(get_line_number(line))
        elif line.startswith("(y)"):
            y_ls.append(get_line_number(line))
            # get the score
            tmp_score = 0
            if "#" in line:
                tmp_score = int(line.split("#")[-1])
            scores += tmp_score
        elif line.startswith("x"):
            x_line_number = l
        elif line.startswith("y"):
            y_line_number = l
        elif line.startswith("Scores"):
            score_line_number = l
    #print("x_ls: ", x_ls)
    #print("y_ls: ", y_ls)
    #print(f"-> todolist: {scores} points")
    # rewirte the todolist
    lines[x_line_number] = f"{'x':^6}:  {', '.join(x_ls)}\n"
    lines[y_line_number] = f"{'y':^6}:  {', '.join(y_ls)}\n"
    lines[score_line_number] = f"{'Scores':6}:  {scores} points\n"
    with open(todolist_path, "w", encoding="utf-8") as f:
        f.write("".join(lines))
    return scores

def get_question_and_learn_score(question_and_learn_dir, full_score_number=10):
    # {{{
    detect_number = 0
    fl_ty = ["question", "learn"]
    # match pattern
    pattern = "\d{1,2} *\."
    for file in fl_ty:
        file_path = os.path.join(question_and_learn_dir, file + ".txt")
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()
        match = re.findall(pattern, content)
        #print(match)
        match_num = 0
        if match:
            match_num = len(match)
            #print(f"{file}: match number: {match_num}")
            # tuncate
            if match_num > full_score_number // len(fl_ty):
                match_num = full_score_number // len(fl_ty)
        # renew the detect_number
        detect_number += match_num
    # get the score
    scores = int(100 // full_score_number * detect_number)
    #print(f"-> questions & learn: {scores} points")
    return scores# }}}


if __name__ == "__main__":
    # deal with the path
    today_date = time.strftime("%Y-%m/%m-%d")
    everyday_record_dir = r"/home/hgj/mygithub/everyday-record/"
    question_and_learn_dir = os.path.join(everyday_record_dir, today_date)
    study_app_dir = r"/cygdrive/c/Users/hgj/Desktop/study-app/data/everyday/"
    arragement_path = os.path.join(study_app_dir, time.strftime("%Y"), today_date, "plan.txt")
    record_path = os.path.join(study_app_dir, time.strftime("%Y"), today_date, "record.txt")
    todolist_path = os.path.join(everyday_record_dir, today_date, "todolist.txt")
    # the question and learn
    q_l_score = get_question_and_learn_score(question_and_learn_dir)
    # the arragement score
    arragement_score = get_arragement_score(arragement_path)
    # the record
    record_score = get_record_score(record_path)
    # the todolist
    todolist_score = get_todolist_score(todolist_path)

    # the total score
    score_ls = [arragement_score, record_score, todolist_score, q_l_score]
    score_weight_ls = [0.4, 0.2, 0.2, 0.2]
    total_score = 0
    for i in range(len(score_ls)):
        total_score += score_ls[i] * score_weight_ls[i]
    total_score = int(total_score)
    def Rank(s):
        # the rank
        if s < 60:
            rank = "BAD" # RED
            rank = color(light_red, "BAD")
        elif s >= 60 and s < 80:
            #rank = "NICE" # ORANGE
            rank = color(yellow, "NICE")
        elif s >= 80 and s < 90:
            rank = "GOOD" # BLUE
            rank = color(light_blue, "GOOD")
        else:
            rank = "EXCELLENT" # GREEN
            rank = color(light_green, "EXCELLENT")
        #rank = "(" + rank + ")"
        return rank

    def space(num):
        return " "*num
    # show the result
    total_score_str = "\033[46;31mTOTAL SCORE\033[0m"
    first_line = f"-------------------------Today Performance-------------------------\n\n"
    show_content = first_line
    show_content += f"{total_score_str}{space(22-len('total score'))}:{space(4)}{total_score}  {Rank(total_score)}\n\n" \
                f"{'arragement score':<22}:{space(4)}{arragement_score}  {Rank(arragement_score)}\n\n" \
                f"{'record score':<22}:{space(4)}{record_score}  {Rank(record_score)}\n\n" \
                f"{'todolist score':<22}:{space(4)}{todolist_score}  {Rank(todolist_score)}\n\n" \
                f"{'question&learn score':<22}:{space(4)}{q_l_score}  {Rank(q_l_score)}\n\n" \
                f"{'-'*len(first_line)}\n"
    print(show_content)
