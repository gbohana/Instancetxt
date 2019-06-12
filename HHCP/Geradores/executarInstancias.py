
"run vw with various bits settings, save reported losses to a csv file"
"subprocess.check_output() version"
import re
import csv
import subprocess
import os

path = '/home/gabriela/INSTANCES FINAL/'
output_file = '/home/gabriela/NurseRostering/HHCP/Geradores/res.csv'
cmd = './MiniZincIDE.sh HCS_Novo.mzn'
program = "MiniZincIDE.sh"

def get_dist( output ):
    pattern = 'Distancia = (.*?)\n'
    m = re.search( pattern, output )
    dist = m.group( 1 )
    return dist


def get_vis( output ):
    pattern = 'Visitas = (.*?)\n'
    m = re.search( pattern, output )
    vis = m.group( 1 )
    return vis

o_f = open( output_file, 'wb' )
writer = csv.writer( o_f )
writer.writerow( [ 'visitas', 'distancias' ] )

for filename in os.listdir(path):
    # do your stuff
    subprocess.call([cmd, filename])
    vis = get_dist(output)
    dist = get_vis(output)
    writer.writerow( [ vis, dist ] )
    o_f.flush()
