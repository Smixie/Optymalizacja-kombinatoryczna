import os
import subprocess
import time

path = "25"

res = []

for path2 in os.listdir(path):
    if os.path.isfile(os.path.join(path, path2)):
        res.append(path2)

for x in res:
    filename = f'25/{x}'
    print(filename, end="\n")
    subprocess.run(["lol.exe", filename, "output.txt"])
    subprocess.run(["ckrptw.exe", filename, "output.txt"])