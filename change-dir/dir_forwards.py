#! python
import os
import re
import sys

args = sys.argv[1: ]
if not args:
    print(".")
    exit(0)

# get all the dir path
for root, dirs, files in os.walk("."):
    # check the root
    is_meet_flag = True
    root_and_dir_ls = [root] + [root+"/"+i for i in dirs]
    for root_and_dir in root_and_dir_ls:
        is_root_and_dir_meet_need = True
        for arg in args:
            if arg not in root_and_dir:
                is_root_and_dir_meet_need = False
                break
        # check
        if is_root_and_dir_meet_need:
            print(root_and_dir)
            exit(0)
# can not search
print(".")
