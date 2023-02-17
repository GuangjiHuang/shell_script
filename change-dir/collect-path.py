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
                dir_path_ls.append(path)
                getDirPath(path, dir_path_ls, depth+1, depth_max)
    except:
        pass
    return dir_path_ls

base_root = "../"
dir_path_ls = [base_root]
dir_path_ls = getDirPath(base_root, dir_path_ls, 1, 1)
for dir_path in dir_path_ls:
    print(dir_path)
print(len(dir_path_ls))


# get the args
#if len(sys.argv) == 1:
#    args = ["0"]
#else:
#    args = sys.argv[1:]
#
## the file path to store the information
#home_path = os.getenv("HOME")
#script_dir = "mygithub/shell-script/change-dir"
#file_path = os.path.join(home_path, script_dir, "most-often-used-path.pkl")
##
#first_arg = args[0]
#if first_arg.isdigit():
#    # save the current path or the sub path to the file
#
#    pass
#else:
#    # search and then jump to the path
#    pass
#
#
## save the dir to the file
#path_dict = {"/home": 1, 
#        "/home/ysq": 2
#        }
#with open(file_path, "wb") as f:
#    pickle.dump(path_dict, f)
#
## load the file
#with open(file_path, "rb") as f:
#    l_path_dict = pickle.load(f)
#print(l_path_dict)
