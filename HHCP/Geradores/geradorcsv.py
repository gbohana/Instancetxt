import re
import csv
import subprocess
import os
import os.path

path_to_ins = 'C:\\Users\\gabi_\\Documents\\Sols\\'
path_to_csv = 'C:\\Users\\gabi_\\Documents\\'

with open(path_to_csv + 'res.csv', 'w') as writeFile:
    writer = csv.writer(writeFile)
    
    for filename in os.listdir(path_to_ins):
        # executes program passing files as parameters
        arq = filename
        arq = arq[14:-4] # 3_10_0_23
        arq = arq.split("_")
        # print( str(arq) )
        with open(path_to_ins+filename, 'r') as readFile:
            reader = csv.reader(readFile)
            lines = list(reader)
            visitas = str(lines[0])
            print(visitas)
            visitas = visitas[13:-4]
            distancia = str(lines[1])
            print(distancia)
            distancia = distancia[15:-3]
            print(visitas, distancia)
            # 11 13
            arq.append(visitas) # concatenar à lista!!
            arq.append(distancia)
        writer.writerow(arq) # writes 3, 10, 0, 23
        readFile.close()
        

writeFile.close()



    
