import os
import sys
import pickle
import sys
import time

def compare(s1, s2, compare_level=3):
    """
    compare_level: 
        0: ignore the case
        1: case sensitive
        2: dir name full match, ignore case
        3: case sensitive and dir name full match
    """
    if compare_level == 0:
        return s1.lower() in s2.lower()
    elif compare_level == 1:
        return s1 in s2
    elif compare_level == 2:
        return ("/"+s1+"/").lower() in s2.lower()
    else:
        return "/"+s1+"/" in s2

def viewPath(pkl_path, save_path):
    with open(pkl_path, "rb") as f:
        dir_n_dict = pickle.load(f)
    dir_ls = list()
    for dir_, _ in dir_n_dict.items():
        dir_ls.append(dir_)
    # save to the file
    with open(save_path, "w", encoding="utf-8") as f:
        f.write("\n".join(dir_ls))
    # view by vim
    # os.system(f"vim {save_path}")

def is_ignore(path, ignore_lines):
    for line in ignore_lines:
        if "/"+line+"/" in path:
            return True
    return False

def getDirPath(base_path, dir_path_ls, depth, depth_max, ignore_lines):
    try:
        for item in os.listdir(base_path):
            path = os.path.join(base_path, item)
            if os.path.isdir(path) and depth <= depth_max and not is_ignore(path, ignore_lines):
                # append the path to list
                dir_path_ls.append(os.path.abspath(path)+"/")
                getDirPath(path, dir_path_ls, depth+1, depth_max, ignore_lines)
    except:
        pass

# get the args
if len(sys.argv) == 1:
    args = ["1"]
else:
    args = sys.argv[1:]

# the file path to store the information
home_path = os.getenv("HOME")
script_dir = "mygithub/shell-script/change-dir"
file_path = os.path.join(home_path, script_dir, "most-often-used-path.pkl")
if not os.path.exists(file_path):
    with open(file_path, "wb") as f:
        pickle.dump({"^&#%": 0}, f)
#
first_arg = args[0]
is_sorted = False
if first_arg.isdigit():
    t_s = time.time()
    search_depth = int(first_arg)
    # save the current path or the sub path to the file
    base_root = os.getcwd()
    dir_path_ls = [base_root+"/"]
    print(f"Get all the sub dir path of the {base_root} ... waiting... ")
    # get the ignore_lines
    exclude_dir_path = os.path.join(home_path, script_dir, "collect.ignore")
    if os.path.exists(exclude_dir_path):
        with open(exclude_dir_path, "r", encoding="utf-8") as f:
            ignore_lines = [line.strip() for line in f.readlines()]
    else:
        with open(exclude_dir_path, "w", encoding="utf-8") as f:
            pass
        ignore_lines = list()
    getDirPath(base_root, dir_path_ls, 1, search_depth, ignore_lines)
    print("Save the dir path to the pickle ... ", end=" ")
    with open(file_path, "rb") as f:
        l_path_dict = pickle.load(f)
    for dir_path in dir_path_ls:
        if dir_path in l_path_dict:
            l_path_dict[dir_path] += 1
        else:
            l_path_dict.update({dir_path: 1})
    if is_sorted:
        l_path_dict_ls = sorted(l_path_dict.items(), key=lambda x: x[1], reverse=True)
        l_path_dict.clear()
        l_path_dict.update(l_path_dict_ls)
    
    with open(file_path, "wb") as f:
        pickle.dump(l_path_dict, f)
    # 
    print(f"Save number:{len(dir_path_ls)}, total: {len(l_path_dict)}, time usage: {time.time()-t_s:0.2f}")
elif first_arg == "view":
    save_txt_path = os.path.join(home_path, script_dir, "most-often-used-path.txt")
    viewPath(file_path,save_txt_path)
    print(save_txt_path)
else:
    t_s = time.time()
    # load
    with open(file_path, "rb") as f:
        l_path_dict = pickle.load(f)
    # print and search
    #for k, v in l_path_dict.items():
    #    print(k, ": ", v)
    #print(f"len: {len(l_path_dict)}")
    # search and then jump to the path
    is_search_flag = False
    for k, v in l_path_dict.items():
        is_meet_the_need = True
        for arg in args:
            if not compare(arg, k, compare_level=3):
                is_meet_the_need =False
                break
        if is_meet_the_need:
            print(k, end="")
            is_search_flag = True
            break
    if not is_search_flag:
        print(".", end="")
    print(f"#time usage{time.time()-t_s:0.2f}")
