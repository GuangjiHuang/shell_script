import os
import sys
import pickle
import sys
import time

def getDirPath(base_path, dir_path_ls, depth, depth_max):
    try:
        for item in os.listdir(base_path):
            path = os.path.join(base_path, item)
            if os.path.isdir(path) and depth <= depth_max:
                # append the path to list
                dir_path_ls.append(os.path.abspath(path))
                getDirPath(path, dir_path_ls, depth+1, depth_max)
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
if first_arg.isdigit():
    t_s = time.time()
    search_depth = int(first_arg)
    # save the current path or the sub path to the file
    base_root = os.getcwd()
    dir_path_ls = [base_root]
    getDirPath(base_root, dir_path_ls, 1, search_depth)
    #print("save the dir path to the pickle")
    with open(file_path, "rb") as f:
        l_path_dict = pickle.load(f)
    for dir_path in dir_path_ls:
        if dir_path in l_path_dict:
            l_path_dict[dir_path] += 1
        else:
            l_path_dict.update({dir_path: 1})
    l_path_dict_ls = sorted(l_path_dict.items(), key=lambda x: x[1], reverse=True)
    l_path_dict.clear()
    l_path_dict.update(l_path_dict_ls)
    #
    with open(file_path, "wb") as f:
        pickle.dump(l_path_dict, f)
    # 
    #print(f"pickle length: {len(dir_path_ls)}, time usage: {time.time()-t_s:0.2f}")
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
            if arg not in k:
                is_meet_the_need =False
                break
        if is_meet_the_need:
            #print("search path: ", k)
            print(k, end="")
            is_search_flag = True
            break
    if not is_search_flag:
        print(".", end="")
    print(f"#time usage{time.time()-t_s:0.2f}")
