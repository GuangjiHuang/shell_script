#! python
import os
import re
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

t_s = time.time()
args = sys.argv[1: ]
if not args:
    print(f".#{time.time()-t_s:.1f}s")
    exit(0)

# get all the dir path
#
for root, dirs, files in os.walk("."):
    # check the root
    is_meet_flag = True
    #root_and_dir_ls = [root] + [root+"/"+i for i in dirs]
    root_and_dir_ls = [root]
    for root_and_dir in root_and_dir_ls:
        is_root_and_dir_meet_need = True
        for arg in args:
            if arg not in root_and_dir.lower():
                is_root_and_dir_meet_need = False
                break
        # check
        if is_root_and_dir_meet_need:
            print(f"{root_and_dir}#{time.time()-t_s:.1f}s")
            exit(0)
        # 
# can not search
print(f".#{time.time()-t_s:.1f}s")
