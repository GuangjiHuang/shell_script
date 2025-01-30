import os
import sys
import pickle
import sys
import time

def get_cwd():
     return os.environ["PWD"]

def compare(s1, s2, compare_level=3):
    """
    compare_level: 
        0: ignore the case
        1: case sensitive
        2: dir name full match, ignore case
        3: case sensitive and dir name full match
    """
    if "*" in s1:
        idx_star = s1.find("*")
        return "/"+s1[:idx_star] in s2 and s1[idx_star+1:] in s2
    else:
        if compare_level == 0:
            return s1.lower() in s2.lower()
        elif compare_level == 1:
            return s1 in s2
        elif compare_level == 2:
            return ("/"+s1+"/").lower() in s2.lower()
        else:
            return "/"+s1+"/" in s2
def cal_score(dir_name_ls, full_path):
    dir_ls = full_path.split("/")
    dir_ls.reverse()
    dir_name_ls.reverse()

    score = 0
    idx_ls = list()

    for dir_name in dir_name_ls:
        if dir_name in dir_ls:
            idx = dir_ls.index(dir_name)
            dir_ls.remove(dir_name)
            idx_ls.append(idx)
        else:
            return 0
    for idx in idx_ls:
        score += 100 - 10 * idx

    return score / len(dir_name_ls)

def compare_v2(dir_name_ls, full_path_ls):
    path_score_dict = dict()
    for full_path in full_path_ls:
        full_path = full_path.strip()
        score = cal_score(dir_name_ls, full_path)
        if score > 0:
            path_score_dict.update({full_path: score})
    # sort
    sorted_data = sorted(path_score_dict.items(), key=lambda item: (-item[1], len(item[0])))
    sorted_data = dict(sorted_data)

    final_path = "."

    cnt = 0

    for key, val in sorted_data.items():
        if cnt == 0:
            final_path = key
        #if cnt < 10:
        #    print(key, val)
        cnt += 1

    return final_path



def viewPath(pkl_path, save_path):
    with open(pkl_path, "rb") as f:
        dir_n_dict = pickle.load(f)
    dir_ls = list()
    for dir_, _ in dir_n_dict.items():
        dir_ls.append(dir_)
    with open(save_path, "w", encoding="utf-8") as f:
        f.write("\n".join(dir_ls))

def is_exclude(path, exclude_lines):
    for line in exclude_lines:
        if "/"+line+"/" in path:
            return True
    return False

def is_include(path, include_lines):
    for line in include_lines:
        if "/"+line+"/" in path:
            return True
    return False

def get_depth(current_dir, base_dir):
    rel_path = current_dir.replace(base_dir, "")
    depth = len(rel_path.split(os.sep))

    return depth



def getDirPath(base_path, dir_path_ls, depth, depth_max, exclude_lines, include_lines):
    for item in os.listdir(base_path):
        path = os.path.join(base_path, item)
        if os.path.isdir(path) and depth <= depth_max and not is_exclude(path, exclude_lines):
            if is_include(path, include_lines):
                dir_path_ls.append(os.path.abspath(path)+"/")
            getDirPath(path, dir_path_ls, depth+1, depth_max, exclude_lines, include_lines)

def getDirPath_v2(base_path, dir_path_ls, depth_, depth_max, exclude_lines, include_lines):
    #for root, dirs, files in os.walk(base_path, followlinks=True):
    for root, dirs, files in os.walk(base_path, followlinks=False):
        for dir_name in dirs:
            dir_path = os.path.join(root, dir_name)
            depth = get_depth(dir_path, base_path)

            #if depth <= depth_max and not is_exclude(dir_path, exclude_lines) and is_include(dir_path, include_lines):
            #if depth <= depth_max: #and not is_exclude(base_path, exclude_lines) and is_include(base_path, include_lines):
            if not is_exclude(dir_path, exclude_lines) and is_include(dir_path, include_lines):
                dir_path_ls.append(dir_path)



if len(sys.argv) == 1:
    args = ["1"]
else:
    args = sys.argv[1:]

home_path = os.getenv("HOME")
script_dir = "mygithub/shell-script/change-dir"
file_path = os.path.join(home_path, script_dir, "most-often-used-path.pkl")

if not os.path.exists(file_path):
    with open(file_path, "wb") as f:
        pickle.dump({"^&#%": 0}, f)

first_arg = args[0]
is_sorted = False

if first_arg.isdigit():
    t_s = time.time()
    search_depth = int(first_arg)

    base_root = get_cwd()
    dir_path_ls = [base_root+"/"]
    print(f"Get all the sub dir path of the {base_root} ... waiting... ")

    collect_cfg_path = os.path.join(home_path, script_dir, "collect.cfg")

    exclude_lines = list()
    include_lines = list()
    if os.path.exists(collect_cfg_path):
        with open(collect_cfg_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
        for line in lines:
            line = line.strip()
            if len(line) == 0:
                continue
            if line.startswith("["):
                if "exclude" in line:
                    tmp_ls = exclude_lines
                elif "include" in line:
                    tmp_ls = include_lines
                continue
            tmp_ls.append(line)
            
    else:
        with open(collect_cfg_path, "w", encoding="utf-8") as f:
            pass
    #print(f"exclude ls: {exclude_lines}")
    #print(f"include ls: {include_lines}")

    #getDirPath(base_root, dir_path_ls, 1, search_depth, exclude_lines, include_lines)
    getDirPath_v2(base_root, dir_path_ls, 1, search_depth, exclude_lines, include_lines)
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

    print(f"Save number:{len(dir_path_ls)}, total: {len(l_path_dict)}, time usage: {time.time()-t_s:0.2f}")
elif first_arg == "view":
    save_txt_path = os.path.join(home_path, script_dir, "most-often-used-path.txt")
    viewPath(file_path, save_txt_path)
    print(save_txt_path)
else:
    t_s = time.time()
    with open(file_path, "rb") as f:
        l_path_dict = pickle.load(f)
        l_path = list(l_path_dict.keys())
    final_path = compare_v2(args, l_path)
    print(final_path, end="")
    """
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
    """
    print(f"#time usage{time.time()-t_s:0.2f}")
