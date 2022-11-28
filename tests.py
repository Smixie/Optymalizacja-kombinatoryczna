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
    subprocess.run(["./pod testy/program.exe", filename, "./pod testy/output.txt"])
    subprocess.run(["ckrptw.exe", filename, "./pod testy/output.txt"])