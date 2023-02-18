import os

# set the environment variable

os.environ["CD_PATH"] = "/home/gjsq/mygithub"
print(os.getenv("CD_PATH"))
