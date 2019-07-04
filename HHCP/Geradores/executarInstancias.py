
"run vw with various bits settings, save reported losses to a csv file"
"subprocess.check_output() version"
import re
import csv
import subprocess
import os

path_to_dzn = '/home/gabriela/INSTANCES FINAL/'
path_to_mzn = '/home/gabriela/NurseRostering/HHCP/HCS_Novo.mzn'

for filename in os.listdir(path_to_dzn):
    # executes program passing files as parameters
    # # subprocess.call([cmd, filename])
    arq = filename
    if arq.endswith('.dzn'):
        arq = arq[:-4]
    #print(arq + " "+ filename)
    cmd = './minizinc --solver Gurobi /home/gabriela/NurseRostering/HHCP/HCS_Novo.mzn /home/gabriela/INSTANCES\ FINAL/' + filename +' -o /home/gabriela/INSTANCES\ FINAL/' + arq + '.sol'
    os.system( cmd )

#./minizinc --solver Gurobi ~/NurseRostering/HHCP/HCS_Novo.mzn ~/INSTANCES\ FINAL/Instanz_HCSRP_3_10_0_0.dzn -o saida.txt
