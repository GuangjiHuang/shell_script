#! python
import os
import re
import sys

if len(sys.argv) > 1:
    arg = sys.argv[1]
else:
    # back to the last dir
    arg = "1"

# get the current dir
cwd = os.getcwd()
dir_ls = cwd.split("/")
# if the arg is the digit
if arg.isdigit():
    back_path = "/".join(dir_ls[:-1-int(arg)])
    print(back_path)
else:
    for i, dir in enumerate(dir_ls):
        mt = re.match(arg, dir, re.I)
        if mt:
            # stop and then jump to the dir
            back_path = "/".join(dir_ls[:i+1])
            print(back_path)
