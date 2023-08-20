import subprocess
from multiprocessing import Pool
import numpy as np


widths = [x*1.2 for x in range(1,5)] # cm
heights = [x*20 for x in range(1,4)] # cm
step_z = 10 # mm
params = []

counter = 0
for width in widths:
    for height in heights:
        for z in np.arange(0,10*height/2, step_z):
            params.append((width,height,z,counter))
            with open(f'build/macros/beamMuon_{counter}.mac','w') as f:
                text = f"""/control/verbose 0
/tracking/verbose 0
/event/verbose 0
/run/initialize
/gps/verbose 0
/gps/particle mu+
/gps/ene/mono 1 GeV
/gps/direction 0 -1 0
/gps/position 0 30 {z} cm
/run/beamOn 500"""
                f.write(text)
            counter +=1



def foo(args):
    return subprocess.run(["build/OpNovice",'-width',str(args[0]),'-height',str(args[1]),'-m',f'build/macros/beamMuon_{args[3]}.mac'])

with Pool(4) as pool:
    pool.map(foo, params)

#for arg in params:
#    foo(arg)