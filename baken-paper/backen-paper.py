import os
import yaml
import time

file_dir = "/home/gjsq/mygithub/shell-script/baken-paper"
os.chdir(file_dir)

# load the dir_ls
file_path = r"dir-config.yml"
with open(file_path, "r") as f:
    dir_dict = yaml.load(f, Loader=yaml.FullLoader)
source_dir_ls = dir_dict["source_dir_ls"]
target_dir_ls = dir_dict["target_dir_ls"]

# get the date of today
today = time.strftime("%m/%m-%d")
for target_dir in target_dir_ls:
    os.makedirs(os.path.join(target_dir, today), exist_ok=True)

# copy the dir backen
for source_dir in source_dir_ls:
    for target_dir in target_dir_ls:
        os.system(f"cp -r {source_dir} {os.path.join(target_dir, today)}")
        print(f"{source_dir} -> {os.path.join(target_dir, today)}")
