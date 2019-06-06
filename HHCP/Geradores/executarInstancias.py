import os
import subprocess

#for loop pra executar (3 x 3 x 11) x 30 vezes

path = '/home/gabriela/INSTANCES FINAL/'
program = "MiniZincIDE.sh"

for filename in os.listdir(path):
    # do your stuff
    subprocess.call([program, filename])

#subprocess.call(["test.exe", files[i]])

# for i in range (len(files)):
#     # em python 3.5 usa run
#     subprocess.call([program, files[i]])

# with open('argumentos.txt','r') as f:
#     for line in f:
#         for x in range(30):
#             l = line.split()
#             l.append(str(x))
#             subprocess.Popen([program]+l)
