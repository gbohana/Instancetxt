import os
import subprocess

#for loop pra executar (3 x 3 x 11) x 30 vezes

program = './a.out'
x = 0
with open('argumentos.txt','r') as f:
    for line in f:
        for x in range(30):
            l = line.split()
            l.append(str(x))
            subprocess.Popen([program]+l)

