import os
import glob
import re

if __name__ == "__main__":
    # get the xxx.stdtemplate first
    template_file_list = glob.glob("*_stdtemplate.*")
    if template_file_list:
        template_file = template_file_list[0]
        template_file_prefix = template_file.split("_")[0] 
        template_file_suffix = template_file.split(".")[1]
    else:
        print("No the template file!")
        exit(1)

    # get the all file to the list
    all_files = os.listdir("./")

    # match them and them get the number
    pattern = rf"{template_file_prefix}_\d+\.{template_file_suffix}"
    #pattern = r"opencv_\d+\.cpp"
    all_files_match = [i for i in all_files if re.match(pattern, i)]
    all_files_number = []
    for i in all_files_match:
        num_list = re.findall(r"_(\d+)\.", i)
        if num_list:
            num = int(num_list[0])
            all_files_number.append(num)

    # extract the max number, and create the new file name
    max_num = max(all_files_number) if all_files_number else 0 #if empty assign to the 0
    new_num = max_num + 1
    new_file_name = f"{template_file_prefix}_{new_num}.{template_file_suffix}"

    # create the new file
    bash_command = f"cp {template_file} {new_file_name}"
    os.system(bash_command)
    #print(f"New file -> {new_file_name}")
    print(f"{new_file_name}")
