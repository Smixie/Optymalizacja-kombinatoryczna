import os
import subprocess
import time

path = "2000"

res = []

for path2 in os.listdir(path):
    if os.path.isfile(os.path.join(path, path2)):
        res.append(path2)

for x in res:
    filename = f'{path}/{x}'
    print(filename, end="\n")
    subprocess.run(["./a.exe", filename, "./output.txt"])
    subprocess.run(["ckrptw.exe", filename, "./output.txt"])