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
    try:
        target_ls = dir_ls[:-int(arg)]
        if len(target_ls) == 1:
            print("/")
        else:
            back_path = "/".join(target_ls)
            print(back_path)
    except:
        print(cwd)
else:
    if arg == "/":
        print("/")
        exit(0)
    elif arg == "-":
        print("-")
        exit(0)
    is_found = False
    for i, dir in enumerate(dir_ls):
        try:
            #mt = re.match(arg, dir, re.I)
            #if mt:
            if arg.lower() in dir.lower():
                # stop and then jump to the dir
                back_path = "/".join(dir_ls[:i+1])
                print(back_path)
                is_found = True
                break
        except:
            pass
    # not find, stay in the current dir
    if not is_found:
        print(cwd)
