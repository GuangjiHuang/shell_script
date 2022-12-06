import json
import os
import shutil
import sys
import platform
import argparse
import readline

# about the color   
# the color
no_color = "\033[0m"
dark = "\033[0;30m"
light_dark = "\033[1;30m"
red = "\033[0;31m"
light_red = "\033[1;31m"
green = "\033[0;32m"
light_green = "\033[1;32m"
orange = "\033[0;33m"
yellow = "\033[1;33m"
blue = "\033[0;34m"
light_blue = "\033[1;34m"
purple = "\033[0;35m"
light_purple = "\033[1;35m"
cayon = "\033[0;36m"
light_cayon = "\033[1;36m"
light_gray = "\033[0;37m"
white = "\033[1;37m"

def color(color_, content):
    return color_ + content + no_color


# the global var
show_label_ls = ["-", "x", "y", "^", "a"]
show_label_ls_verbose = ["Not learn", "Not remember", "Has remembered", "Need enhance", "All"]

# get the alias comand
            
def readAlias(path=None):
    if path == None:
        path = "./alias.txt"
    with open(path, "r") as f:
        line_ls = f.readlines()
    # filter the line
    line_ls = [line.strip(" \n\t") for line in line_ls]
    line_ls = [line for line in line_ls if line != ""]
    line_ls = [line for line in line_ls if not line.isspace() and line[0]!="#"]
    # to dict
    alias_dict = dict()
    for line in line_ls:
        alias_ls = line.split("=")
        if len(alias_ls) != 2:
            continue
        left_cmd = alias_ls[0].strip(" ")
        right_cmd = alias_ls[1].split("#")[0]
        right_cmd = right_cmd.strip(" ")
        alias_dict[left_cmd] = right_cmd
    return alias_dict

    
def commandDeal(command):
    # get rid of the space
    command_ls = command.split(" ")
    command_ls = [command for command in command_ls if command != ""]
    return command_ls

def loadJsonFile(path):
    with open(path, "r+") as f:
        json_obj = json.load(f)
    return json_obj

def saveJsonFile(json_obj, path):
    with open(path, "w+") as f:
        json.dump(json_obj, f)

def loadTxtFile(path):
    with open(path, "r+") as f:
        content = f.read()
    content = content.strip("\n")
    return content

def saveTxtFile(str_obj, path):
    with open(path, "w+") as f:
        f.write(str_obj)

def spaceNum(n):
    return " " * n

def tab(n):
    return "\t" * n

def newline(n):
    return "\n" * n

def hyphen(n):
    return "-" * n

def cls():
    if platform.system() == "Windows":
        os.system("cls")
    else:
        os.system("clear")

def getLabelFromStr(label_str):
    label_format_str = ""
    for label in label_str:
        if label in show_label_ls:
            label_verbose = show_label_ls_verbose[show_label_ls.index(label)]
            label_format_str += f"{label}: {label_verbose}{spaceNum(3)}"
    return label_format_str.strip(" ")

def getshowWordsList(word_dict, chapter, number, label_str):
    chapter_dict = word_dict[chapter]
    if "a" in label_str:
        show_ls = list(chapter_dict.keys())
    else:
        show_ls = [word for word in chapter_dict.keys() if chapter_dict[word]["label"] in label_str]
    number = min(number, len(show_ls))
    show_ls = show_ls[0:number]
    return show_ls

def showWords(word_dict, show_ls, label_str, is_show_newline, is_show_newline_threshold):
    label_count_dict = wordsLabelCount(word_dict)
    total_label_number = 0
    for label in label_str:
        total_label_number += label_count_dict[label]
    show_label_number = len(show_ls)
    # get the label number information
    # show the header
    #header_format = f">>> SHOWING:  ({label}: {label_verbose})  ({show_label_number}/{total_label_number}) <<<"
    header_format = f"[ {color(light_green, 'SHOWING')}:  ({getLabelFromStr(label_str)})  ({show_label_number}/{total_label_number}) "
    print(header_format)
    print(hyphen(55))
    # show the main part
    is_show_newline_after_each_line = True if is_show_newline and len(show_ls)<=is_show_newline_threshold*len(word_dict) else False
    for i, word in enumerate(show_ls, 1):
        line_format = f"{i:>3}{tab(1)}{word:<15}{word_dict[word]['label']}"
        print(line_format)
        if is_show_newline_after_each_line:
            print()

def showHeader(chapter, word_dict):
    #header_format = f"--->>> {chapter}\t"
    header_format = f"[ {color(light_green, 'Count Info')}:  "
    if chapter == "No set":
        header_format += f"x:None\t^:None\ty:None\t-:None\ta:None ]"
    else:
        label_count_dict = wordsLabelCount(word_dict)
        x_n = label_count_dict["x"]
        __n = label_count_dict["^"]
        y_n = label_count_dict["y"]
        h_n = label_count_dict["-"]
        a_n = len(word_dict)
        header_format += f"x:{x_n:>2}{spaceNum(3)}^:{__n:>2}{spaceNum(3)}y:{y_n:>2}{spaceNum(3)}-:{h_n:>2}{spaceNum(3)}a:{a_n:>2} ]"
    print(hyphen(55))
    print(header_format)

def wordsLabelCount(word_dict):
    # creat the dict to store the label number
    show_label_ls = ["-", "x", "y", "^", "a"]
    label_count_dict = dict()
    for label in show_label_ls:
        label_count_dict[label] = 0
    # add
    for word in word_dict:
        local_label = word_dict[word]["label"]
        label_count_dict[local_label] += 1
        label_count_dict["a"] += 1
    return label_count_dict

def argParse():
    parser = argparse.ArgumentParser(description="for English word")
    parser.add_argument("-workspace", help="the dir of the code")
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = argParse()
    # the var
    if args.workspace:
        json_file_path = f"{args.workspace}/all_English_word.json"
        chapter_temp_path = f"{args.workspace}/chapter_temp.txt"
        alias_path = f"{args.workspace}/alias.txt"
        os.chdir(args.workspace)
    else:
        json_file_path = r"./all_English_word.json"
        chapter_temp_path = r"./chapter_temp.txt"
        alias_path = f"./alias.txt"
    # get the alias command
    alias_cmd = readAlias(alias_path)
    # sudo var
    #cmd_first_ls = ["chapter", "show", "label", "trans", "help", "clear", "exit", "save", "execute"]
    show_number = 15
    show_label = "a"
    show_newline = True
    show_newline_thres = 0.3
    # load the json file here
    en_word_dict =  loadJsonFile(json_file_path)
    cpt_list = list(en_word_dict.keys())
    # read the chapter temp file to get the chapter
    word_chapter = loadTxtFile(chapter_temp_path)
    # the show list 
    show_ls = getshowWordsList(en_word_dict, word_chapter, show_number, show_label)
    # the flag of the continue to show
    is_continue_show_flag = False
    # check
    if word_chapter not in cpt_list:
        word_chapter = "No set"
    while True:
        if is_continue_show_flag:
            is_continue_show_flag = False # important to turn off the contunie showing!
            command_ls = ["show"]
        else:
            command = input(f"{newline(1)}[{color(yellow, word_chapter)}] {color(light_blue, 'Please input the command: ')}")
# add the command aliases
            command = alias_cmd.get(command, command)
            command_ls = commandDeal(command)
        command_len = len(command_ls)
        if command_len == 0:
            cls()
            # print the title
            print(f"[ {hyphen(22)} {color(light_cayon, 'English Learning System')} {hyphen(22)} ]")
            continue
        command_first = command_ls[0]
        #
        if command_first in ["chapter", "cpt", "bp", "bn"] or command_first.isdecimal():
            # bp: back to previous, bn: back to next
            if command_first in ["bp", "bn"]:
                word_chapter_num = int(word_chapter.split("_")[-1])
                if command_first == "bp":
                    word_chapter_num -= 1
                else:
                    word_chapter_num += 1
                # renew the command_ls
                command_ls = ["cpt", "cpt_"+f"{word_chapter_num:02}"]
            # if the command_first is the demical, renew the command_ls
            if command_first.isdecimal():
                command_ls = ["cpt", "cpt_"+command_first]
            # if the command_ls is the number, add the prefix: cpt_
            if command_ls[1].isdecimal():
                command_ls[1] = "cpt_" + command_ls[1]
            # check the $1
            if command_ls[1] not in cpt_list:
                #cpt_list.sort()
                print(f"[WW] chapter must be less equal to the {cpt_list[-2]}. cpt 1-9 must be written as the format cpt_01,...cpt_09")
                continue
            # the right chapter, renew the chapter
            if len(command_ls) == 0:
                print(f"[WW] lack the chapter number! Fail!")
                continue
            word_chapter = command_ls[1]
            cls()
            # set the flag to continue to show, and don't need to input the show command
            is_continue_show_flag = True
        # show
        elif command_first in ["show", "s"]:
            # clear
            cls()
            # check the chapter
            if word_chapter == "No set":
                continue
            # show the header
            showHeader(word_chapter, en_word_dict[word_chapter])
            if command_len == 1:
                # just print the word
                show_ls = getshowWordsList(en_word_dict, word_chapter, show_number, show_label)
                showWords(en_word_dict[word_chapter], show_ls, show_label, show_newline, show_newline_thres)
            elif command_len == 3 or command_len == 2 and command_ls[1] == "-n":
                # with -L: label; or the -n: number
                if command_ls[1] == "-l":
                    is_label = True
                    for _ in command_ls[2]:
                        if _ not in show_label_ls:
                            print(f"[WW] label must be one of the {show_label_ls}, instead of the {command_ls[2]}")
                            is_label = False
                            break
                    if not is_label:
                        continue
                    # right label
                    show_ls = getshowWordsList(en_word_dict, word_chapter, show_number, command_ls[2])
                    showWords(en_word_dict[word_chapter], show_ls, command_ls[2], show_newline, show_newline_thres)
                    # by the way, renew the show label
                    show_label = command_ls[2]
                elif command_ls[1] == "-n":
                    # if just the show -n, show the max length
                    if command_len == 2:
                        show_number = 100 #more than the max
                    # try to get the show number
                    elif command_ls[2] == 'h': # mean half
                        show_number = 25
                    else:
                        try:
                            show_number = int(command_ls[2])
                        except:
                            print(f"[WW] show number must be integer, instead of the '{command_ls[2]}'")
                            continue
                    # show
                    show_ls = getshowWordsList(en_word_dict, word_chapter, show_number, show_label)
                    showWords(en_word_dict[word_chapter], show_ls, show_label, show_newline, show_newline_thres)
                else:
                    print(f"[WW] optional argument must be -n or -l, instead of '{command_ls[1]}'")
            else:
                print(f"[WW] show command not right, reference to 'show -n 10' or 'show -l a'")
        #
        elif command_first in ["label", "l"]:
            if command_len != 2 or command_ls[1] not in show_label_ls:
                print("label command is not right!")
                continue
            change_to_label = command_ls[1]
            # the input
            word_to_change = input("[##] >>: ")
            word_to_change_ls = word_to_change.split(" ")
            #word_to_change_ls = [i for i in word_to_change_ls if i != " " and i in en_word_dict[word_chapter]]
            word_to_change_ls_s = list()
            for i in word_to_change_ls:
                if i == "":
                    continue
                # the word
                if i in en_word_dict[word_chapter]:
                    word_to_change_ls_s.append(i)
                # the word id
                elif i.isdecimal():
                    try:
                        word_label_id = int(i)
                        if word_label_id > len(show_ls) or word_label_id <= 0:
                            print(f"[WW] word id must <={len(show_ls)} and >0, instead of the {word_label_id}")
                            continue
                        word_to_change_ls_s.append(show_ls[word_label_id-1])
                    except:
                        pass
                        continue
                # the word id range
                elif ":" in i:
                    try:
                        start, stop = [int(j) for j in i.split(":")]
                        id_range = slice(start-1, stop)
                        word_to_change_ls_s += show_ls[id_range]
                    except:
                        # do nothing if wrong
                        pass
            #
            for word in word_to_change_ls_s:
                en_word_dict[word_chapter][word]["label"] = change_to_label
            # after changing the label, save
            saveJsonFile(en_word_dict, json_file_path)
        #
        elif command_first in ["translate", "trans", "t"]:
            # get the word
            #word = input("[##] >>: ").strip(" ")
            word = command_ls[1]
            # check the word
            if word in en_word_dict[word_chapter]:
                Chinese_str = en_word_dict[word_chapter][word]["translate"]
            elif word.isdecimal():
                try:
                    word_id = int(word)
                    if word_id > len(show_ls) or word_id <= 0:
                        print(f"[WW] word id must <={len(show_ls)} and >0, instead of the {word_id}")
                        continue
                    # get the Chinese string
                    word = show_ls[word_id-1]
                    Chinese_str = en_word_dict[word_chapter][word]["translate"]
                except:
                    print("[WW] word id not right!")
                    continue
            else:
                print(f"[WW] No such word '{word}'")
                continue
            if Chinese_str == "":
                Chinese_str == "None"
            print(f"<< {word} >>")
            print(hyphen(55))
            print(f"{Chinese_str}")

        elif command_first in ["clear", "cl"]:
            # clean the screen
            cls()

        elif command_first == "exit":
            saveJsonFile(en_word_dict, json_file_path)
            saveTxtFile(word_chapter, chapter_temp_path)
            print("[##] Exit!")
            break

        elif command_first in ["save", "s"]:
            saveJsonFile(en_word_dict, json_file_path)
            saveTxtFile(word_chapter, chapter_temp_path)
            print("[##] Has been saved!")

        elif command_first in ["help", "h"]:
            # clean the screen first
            cls()
            print(f"{hyphen(20)} help information {hyphen(20)}\n")
            print(f"-> {'chapter':8}: change the chapter, for example: chapter cpt_01; cpt 01; 78;\n")
            print(f"-> {'show':8}: show the word; example: show -l ^, show -n h, show -n 20. -l: label, -n: number\n")
            print(f"-> {'label':8}: change words label; example: label x, and the next line: xxx(your word)\n")
            print(f"-> {'trans':8}: translate the English word to Chindes; example: trans, and the next line xxx(your word)\n")
            print(f"-> {'clear':8}: clear the screen\n")
            print(f"-> {'save':8}: save your change into the json file\n")
            print(f"-> {'exe':8}: optional argument, -s: the shell command, -p: the python statement, for example: exe -b, and the next line echo hello\n")
            print(f"-> {'sudo':8}: change some show option arguments\n")
            print(f"-> {'->':8}: equal to the bash command: vim temp.txt\n")
            print(f"-> {'help':8}: the help information")

        elif command_first == "backen":
            json_file_bd_path = f"{json_file_path}.bd"
            shutil.copy(json_file_path, json_file_bd_path)
            print("backen successfully!")
        elif command_first in ["execute", "exe"]:
            if command_len == 1:
                print("[WW] You need to slect the option: -p or -s")
                continue
            if command_ls[1] == "-s":
                # execute the bash command, for example: echo hello
                shell_command = input("->(shell command): ").strip(" ")
                os.system(shell_command) 
            elif command_ls[1] == "-p":
                # execute the python command, for example print("hello")
                python_statement = input("->(python statement): ").strip(" ")
                exec(python_statement)
            else:
                print("[WW] not the right command option!")
                continue
        elif command_first in ["->", "vim"]:
            os.system("vim temp.txt")

        elif command_first == "alias":
            if command_len == 1:
                print("[WW] You need to add the option, for example -s")
            else:
                if command_ls[1] == "-s":
                    # show 
                    cls()
                    print(f"{hyphen(20)} ALIASES {hyphen(20)}")
                    for key in alias_cmd:
                        print(f"{key} = {alias_cmd[key]}")
                else:
                    print("[WW] alias command not right!")

        elif command_first == "sudo":
            # show the current information
            print(f"{hyphen(13)} current show argument {hyphen(14)}")
            line_number = f"show_number = {show_number}"
            line_label = f"show_label = {show_label}"
            line_newline = f"show_newline = {show_newline}"
            line_newline_thres = f"show_newline_thres = {show_newline_thres}"
            print(f"| {line_number:47} |")
            print(f"| {line_label:47} |")
            print(f"| {line_newline:47} |")
            print(f"| {line_newline_thres:47} |")
            print(hyphen(50))
            sudo_statement = input("(sudo python statement) >>: ")
            try:
                exec(sudo_statement)
                print("-> ok!")
            except:
                print(f"[WW] execute '{sudo_statement}' fail")
                
        else:
            print(f"[W] '{command_first}' is not the command! Try again!")
