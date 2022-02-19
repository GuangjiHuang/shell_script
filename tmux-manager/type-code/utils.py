import json
import os
import platform

def dumpFile(word_dict, file_path):
    with open(file_path, "w+") as f:
        json.dump(word_dict, f)

def loadFile(file_path):
    with open(file_path, "r+") as f:
        try:
            word_dict = json.load(f)
        except:
            word_dict = {}
    return word_dict
def cls():
    if platform.system() == "Windows":
        clear_command = "cls"
    else:
        clear_command = "clear"
    os.system(clear_command)

def tab(n):
    return "\t" * n

def newline(n):
    return "\n" * n

def hypen(n):
    return "-" * n
