import re
import time
import os

# about the dir and the file type 
up_dir = r"/home/hgj/mygithub/everyday-record"
merge_dir = r"/home/hgj/mygithub/everyday-record/compress"
file_ty_ls = ["learn", "question"]

# read the file, and then get the content
time.strftime("%Y-%m/%m-%d")
today_dir = os.path.join(up_dir, time.strftime("%Y-%m/%m-%d"))
# re pattern 
pattern = r"\d{4}-\d{2}-\d{2}"

# read the merge file, check and then merge
for fl in file_ty_ls:
    # read the file
    fl_path = os.path.join(today_dir, fl+".txt")
    with open(fl_path, "r", encoding="utf-8") as f:
        fl_lines = f.readlines()
        not_empty_lines = [line for line in fl_lines if line.strip()]
        if len(not_empty_lines) < 4:
            print(f"{fl}: not merge!")
            continue
        else:
            print(f"{fl}: merge!")
        fl_content = "".join(fl_lines)
        #print(fl_content)
        match = re.search(pattern, fl_content)
        date = None
        if match:
            date = match.group()
    # read the merge_file
    merge_path = os.path.join(merge_dir, fl+"-a.txt")
    with open(merge_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    # try to find if has the same date in the merge file
    is_in_merge_file = False
    for i in range(len(lines)-1, -1, -1):
        if date in lines[i]:
            is_in_merge_file = True
            break
    if is_in_merge_file:
        with open(merge_path, "w", encoding="utf-8") as f:
            original_content = "".join(lines[:i-1])
            f.write(original_content + fl_content)
    else:
        with open(merge_path, "a", encoding="utf-8") as f:
            f.write(fl_content)
