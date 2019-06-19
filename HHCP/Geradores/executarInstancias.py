
"run vw with various bits settings, save reported losses to a csv file"
"subprocess.check_output() version"
import re
import csv
import subprocess
import os

path_to_dzn = '/home/gabriela/INSTANCES FINAL/'
output_file = '/home/gabriela/NurseRostering/HHCP/Geradores/res.csv'
tmp_log_file = '/home/gabriela/NurseRostering/HHCP/Geradores/tmp_log_file.txt'
path_to_mzn = '/home/gabriela/NurseRostering/HHCP/HCS_Novo.mzn'

def get_dist( output ):
    pattern = 'Distancia = (.*)\n'
    m = re.search( pattern, output )
    dist = m.group( 1 )
    return dist


def get_vis( output ):
    pattern = 'Visitas = (.*)\n'
    m = re.search( pattern, output )
    vis = m.group( 1 )
    return vis

o_f = open( output_file, 'wb' )
writer = csv.writer( o_f )
writer.writerow( [ 'visitas', 'distancias' ] )

for filename in os.listdir(path_to_dzn):
    # executes program passing files as parameters
    # subprocess.call([cmd, filename])
    cmd = '/home/gabriela/MiniZincIDE-2.2.3-bundle-linux/MiniZincIDE.sh  {} {} | tee {}'.format( path_to_mzn, filename, tmp_log_file )
    os.system( cmd )

    output = open( tmp_log_file, 'r' ).read()

    # get data from output
    vis = get_dist(output)
    dist = get_vis(output)

    # writes filtered results in csv file
    writer.writerow( [ vis, dist ] )
    o_f.flush()

# ==> colocar gurobi nos parametros pra ver se vai
# File "executarInstancias.py", line 41, in <module>
#     vis = get_dist(output)
#   File "executarInstancias.py", line 18, in get_dist
#     dist = m.group( 1 )
# AttributeError: 'NoneType' object has no attribute 'group'

#./minizinc --solver Gurobi ~/NurseRostering/HHCP/HCS_Novo.mzn ~/INSTANCES\ FINAL/Instanz_HCSRP_3_10_0_0.dzn -o saida.txt
