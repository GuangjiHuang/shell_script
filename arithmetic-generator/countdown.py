import ctypes
import os
import time
import cv2
import numpy as np

# get the coutdown time
def get_count_down_time(config_path, default_time=60):
    #config_path = f"config_path"
    count_down_time = default_time # minutes

    # check if the file exists
    if os.path.exists(config_path):
        with open(config_path, "r") as f:
            content = f.read()
        content = content.split("#")[0]
        try:
            count_down_time = float(content)
        except:
            print(f"content: {content} is not the number!")
    else:
        print(f"config file: {config_path} not exists! time countdown set to {default_time} minutes.")
    # 
    return int(count_down_time * 60)

def lock_window():
    ctypes.windll.user32.LockWorkStation()

def seconds_to_str(seconds):
    t_m = seconds // 60
    t_s = seconds % 60
    #return f"{t_m:02}:{t_s:02}"
    return f"{t_m:0>2} : {t_s:0>2}"

if __name__ == "__main__":
    config_path = r"./time.txt"
    count_down_time = get_count_down_time(config_path, 0.3)
    is_show_gui = True
    if is_show_gui:
        window_name = "TIME"
        cv2.namedWindow(window_name)
        cv2.setWindowProperty(window_name, cv2.WND_PROP_TOPMOST, 1)
    quit_flag = False
    while count_down_time and (not quit_flag):
        time_str = seconds_to_str(count_down_time)
        print(time_str)
        if is_show_gui:
            count_down_color = (0, 255, 0)
            c_w, c_h = 216, 90
            img_bg = np.zeros((c_h, c_w, 3), np.uint8)
            cv2.putText(img_bg, time_str, (int(0.2*c_w), int(0.5*c_h)), cv2.FONT_HERSHEY_COMPLEX, 1, count_down_color, 3)
            cv2.imshow(window_name, img_bg)
            key_val = cv2.waitKey(1000) & 0xff
            if key_val == ord("q"):
                quit_flag = True
        else:
            time.sleep(1)
        # sub 1
        count_down_time -= 1
    # now is time to lock the win
    if not quit_flag:
        lock_window()
    cv2.destroyAllWindows()
