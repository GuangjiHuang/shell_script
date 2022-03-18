import os
import time
import re

all_file_type = ["task", "record", "learn", "question"]
def getDaysFromDir(dir_path, file_type=None):
    # ret var
    days = list()
    # get the files and dirs under the dir path
    if not os.path.exists(dir_path): # not exist dir path
        return days
    file_dir_path = os.listdir(dir_path)
    for path in file_dir_path:
        sub_dir_path = os.path.join(dir_path, path) # join to the sub dir
        if os.path.isdir(sub_dir_path):
            # check if the file inside the dir
            if (file_type in all_file_type) and (not os.path.exists(os.path.join(sub_dir_path, file_type+".txt"))):
                continue
            day = getDayFromDate(path) # get the day
            if (day is not None) and (day>0 and day<32):  # here to check the day (1-31)
                days.append(day)
    # return the days, list
    return days


def getDayFromDate(date):
    month_day = date.split("-", -1)
    day = None
    try:
        if len(month_day) == 2 and month_day[1].isdecimal():
            day = month_day[1]
            day = int(day)
    except:
        pass
    return day

def printMonthDay(dir_path, year, month, file_type=None, just_calendar=False):
    # -----------------------------
    # the control var
    space_gap = 3
    new_line_gap = 1
    hyphen_num = 45
    # -----------------------------
    # get the specified_day
    dir_path = os.path.join(dir_path, f"{year}", f"{month:02}")
    specified_days = getDaysFromDir(dir_path, file_type);
    # get the weekday of the year-month-1
    wday = None
    time_str = f"{year}-{month}-1"
    time_format = "%Y-%m-%d"
    tm_time = time.strptime(time_str, time_format)
    wday = tm_time.tm_wday  # 0: Monday, .... 6: Sunday
    # get the month's total days
    days = getMonthDays(year, month)
    # creat the print list
    if just_calendar:
        print_ls = list(range(1, days+1))
    else:
        print_ls = [' - '] * days
    for i in specified_days:
        if i>0 and i<=days:
            print_ls[i-1] = str(i) # to location to the i
    # add the prefix to the print_ls
    print_ls = [" "] * wday + print_ls
    # print the month table
    print(hyphen(hyphen_num))
    # print the header
    header_str = f"{3*space(space_gap)}{year}-{month}{space(space_gap)}" \
                 f"[{file_type}:{space(space_gap)}" \
                 f"{len(specified_days)}/{days}]"
    print(header_str)
    print(hyphen(hyphen_num))
    # print the weekday
    weekday_tuple = ("Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun")
    for weekday in weekday_tuple:
        print(space(space_gap), end="")
        print(weekday, end="")
    # print the rest day
    index = 0
    for day in print_ls:
        if index%7==0:
            print(newLine(new_line_gap))
        print(f"{space(space_gap)}{day:^3}", end="")
        index += 1
    print("\n", hyphen(hyphen_num))

def hyphen(num):
    return "-" * num

def space(num):
    return " " * num

def newLine(num):
    return "\n" * num

def getMonthDays(year, month):
    assert (month>0 and month<13)
    is_leap = (year%4==0 and year%100!=0) or (year%100==0 and year%400==0)
    days = None
    if (month==2):
        days = 29 if is_leap else 28
    elif  month in [1, 3, 5, 7, 8, 10, 12]:
        days = 31
    else:
        days = 30
    return days

def printFileList(dir_path, file_type_str=None, states_str=None):
    # str -> list, parse the argument
    file_type_list = None
    states_ls = None
    if file_type_str is not None:
        file_type_list = file_type_str.split(",", -1)
    if states_str is not None:
        states_ls = states_str.split(",", -1)
    #
    if file_type_list == None:
        file_type_list = ["task", "record", "question", "learn"]
    if states_ls == None:
        states_ls = ["x", "-", "y"]
    # get the files list
    file_ls = list()
    if os.path.exists(dir_path):
        files_ls = os.listdir(dir_path)
    # join to the file path
    file_path_ls = list()
    file_path_ls = [os.path.join(dir_path, i) for i in files_ls]
    # for each file, read the state
    print_str = str()
    print_str += hyphen(55)
    print_str += "\n"
    for num_id, file_path in enumerate(file_path_ls, 1):
        states_dict = getFileStates(file_path, states_ls)
        base_name = os.path.basename(file_path)
        file_type, _ = os.path.splitext(base_name)
        # renew the print_str
        states_dict_str = formatStatesShow(states_dict, states_ls)
        print_str += f"{num_id:2}. {file_type:12}{states_dict_str}{newLine(2)}"
    print_str += hyphen(55)
    print_str += "\n"
    # print the str
    print(print_str)

def getFileStates(file_path, states_ls):
    file_state = dict()
    if not os.path.exists(file_path):
        return file_state
    file_lines = list()
    with open(file_path, "r") as f:
        file_lines = f.readlines()
    for line in file_lines:
        if line.isspace():
            continue
        state = extractLineState(line, states_ls) # get the line state
        if state in file_state.keys(): # renew the state dict
            file_state[state] += 1
        else:
            file_state[state] = 1
    return file_state

def extractLineState(line, states_ls):
    state = "x"     # x means not do
    pattern = "\(([a-z])\)"
    match_obj = re.search(pattern, line)
    if match_obj == None:
        return state
    state = match_obj.group(1)
    if state not in states_ls:
        state = "x"
    return state

def formatStatesShow(states_dict, states_ls):
    str_format = str()
    str_format += "[ "
    total_num = 0
    for state in states_ls:
        state_num = states_dict.get(state, 0) # not found, assign to 0
        total_num += state_num
        str_format += f"{state}: {state_num:2}{space(3)}"
    # add the total_num
    str_format += f"total: {total_num:2}"
    str_format += " ]"
    return str_format

def timeDurationFromNow(history_time):
    duration_time = "---"
    # the format: hour:minute
    try:
        h_hour, h_min = history_time.split(":")
        h_hour = int(h_hour)
        h_min = int(h_min)
    except:
        return duration_time
    # get the current time
    cur_tm_time = time.localtime()
    cur_hour = cur_tm_time.tm_hour
    cur_min = cur_tm_time.tm_min
    # sut to minute
    sub_min = (cur_hour * 60 + cur_min) - (h_hour * 60 + h_min)
    if sub_min < 60:
        duration_time = f"{sub_min} min"
    else:
        # convert to the hour
        duration_time = f"{sub_min/60:.1f} hour"
    return duration_time


if __name__ == "__main__":
    #days = [1, 3, 2, 9, 4, 2, 3, 1, 22, 31, 11, 14]
    #days = getDaysFromDir("../data/2022/03", "task")
    #printMonthDay("../data", 2022, 3, "question")
    #dir_path = "../data/2022/03/03-11"
    #printFileList(dir_path)
    history_time = "22:11"
    print(timeDurationFromNow(history_time))




















