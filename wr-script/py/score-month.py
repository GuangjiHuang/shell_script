import time
import os

def space(n):
    return " " * n

def hypen(n):
    return "-" * n

def getDaysOfMonth(year, month):
    #print(year, month)
    if month in [1, 3, 5, 7, 8, 10, 12]:
        return 31
    elif month in [4, 6, 9, 11]:
        return 30
    else:
        if (year%4==0 and year%100!=0) or year % 400==0:
            # is leap year
            return 29
        else:
            return 28

def getScores(record_up_dir, year, month, today_day):
    days = getDaysOfMonth(year, month)
    score_ls = list()
    for day in list(range(1, days+1)):
        if day > today_day:
            score_ls.append("-")
            continue
        day_path = f"{year}-{month:02}/{month:02}-{day:02}/score.txt"
        score_file_path = os.path.join(record_up_dir, day_path)
        if not os.path.exists(score_file_path):
            score_ls.append("-")
        else:
            with open(score_file_path, "r", encoding="utf-8") as f:
                content = f.read().strip()
            #
            score_ls.append(content)
    return score_ls

# print the month
today_tm = time.localtime()
month_day = today_tm.tm_mday
month_first_day_ts = time.time() - (month_day-1) * 24 * 3600
month_first_day_tm = time.localtime(month_first_day_ts)
w_day = month_first_day_tm.tm_wday

year, month, today_day = today_tm.tm_year, today_tm.tm_mon, today_tm.tm_mday
# here put the score list
month_days = getDaysOfMonth(year, month)
day_ls = list(range(1, month_days+1))
day_ls = [str(i) for i in day_ls]
# here put the score list

fill_blank = [" "] * w_day
day_ls = fill_blank + day_ls
#print(len(day_ls))
week_heads = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]

# print the head of the month and the day
h_n = 19
print(f"{hypen(h_n)} {year}-{month} {hypen(h_n)}")
print()
# 
print_ls = week_heads + day_ls
for i, st in enumerate(print_ls):
    print(f"{st:^7}", end="")
    if i % 7 == 6:
        print("\n")
# print the get score's content
record_up_dir = os.path.join(os.getenv("HOME"), "mygithub/everyday-record")
score_content = getScores(record_up_dir, year, month, today_day)
print(score_content)
