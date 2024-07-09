import os
import sys

def getLines(record_path):
    with open(record_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    lines = [line.strip() for line in lines if line.strip()]
    return lines

def reOrder(record_path):
    lines = getLines(record_path)
    for i, line in enumerate(lines):
        if i==0:
            lines[0] = str(len(lines) - 1)
        else:
            idx_dot = line.index(".")
            line = str(i) + ". " + line
            lines[i] = line
    with open(record_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

# get the argument
if len(sys.argv) == 1:
    op = "l"
else:
    op = sys.argv[1]

# get the file path
record_path = r"~/opt/myscript/p-script/record-path.txt"

if op == "l":
    lines = getLines(record_path)
    for i, line in enumerate(lines):
        if i==0:
            continue
        else:
            print(line)

elif op.isdigit():
    num = int(op)
    # change dir to the path
    lines = getLines(record_path)
    if len(lines) == 1:
        print("No path to select! Please use the: <py w [pos]> command to add the path as the record!")
        exit(0)
    if num <=0 or num > len(lines) - 1:
        print(f"Not the right number: {num}, must in the range 1~{len(lines)}")
    else:
        line = lines[num]
        idx = line.find(".")
        line = line[idx+1:].strip()
        os.chdir(line)
        print(line)

elif op == "w":
    # get the current dir
    current_dir = os.getcwd()
    lines = getLines(record_path)
    lines[0] = str(int(lines[0]) + 1)
    # you can specify the position after w, for example: py w 1  
    if len(sys.argv) >= 3 and sys.argv[2].isdigit() and len(lines)>1:
        # can not be at the pos 0
        while pos < 0:
            pos += len(lines)
        if pos == 0:
            print("-> Error: can not insert the current dir in the first line!")
        else:
            pos = int(sys.argv[2])
            lines.insert(pos, current_dir)
            reOrder(record_path)
    else:
        current_dir = str(len(lines)-1) + ". " + current_dir
        lines.append(current_dir)
    # rewrite the file
    with open(record_path, "w") as f:
        content = "\n".join(lines)
        f.write(content)

elif op == "d":
    # delete
    if len(sys.argv) >= 3:
        may_be_number_ls = sys.argv[3:]
        number_ls = list()
        for num in may_be_number_ls:
            if num.isdigit():
                number_ls.append(int(num))
        # 
        lines = getLines(record_path)
        maintain_lines = list()
        for i in range(len(lines)):
            if i == 0 or i not in number_ls:
                maintain_lines.append(lines[0])
        with open(record_path, "w", encoding="utf-8") as f:
            f.write("\n".join(matin_lines))
        # reorder
        reOrder(record_path)
    else:
        print("You have to specify the id number that you want to delete!")

elif op == "m":
    # open in the vim
    command = f"vim {record_path}"
    os.system(command)
    # and then record the file
    reOrder(record_path)

elif op == "py":
    print("-> I don't recommand you use this change command!")

elif op == "go-p":
    dst_path_1 = r"/home/hgj/mygithub/shell-script/p-script/"
    dst_path_2 = r"~/opt/myscript/p-script/"
    if os.path.exists(dst_path_1):
        os.chdir(dst_path_1)
    elif os.path.exists(dst_path_2):
        os.chdir(dst_path_2)
    else:
        print("Error: no the path.sh in your filesystem!")

elif op == "c":
    interaction = input("Are you sure to clear all the record path? (y/n): ")
    if interaction == "y":
        with open(record_path, "w", encoding="utf-8") as f:
            f.write("0")
    else:
        print("Cancel to clear the record path!")

elif op == "help":
    print("this is the help information! Need to be adding at the last time!")

else:
    print("Error: you can use the command <p help> to get the more information!")
