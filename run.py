import subprocess
from multiprocessing import Pool
import numpy as np
import json

widths = [x*1.2 for x in range(1,5)] # cm
heights = [x*15 for x in range(1,7)] # cm
step_z = 10 # mm
params = []

counter = 0
for width in widths:
    for height in heights:
        for z in np.arange(0,10*height/2, step_z):
            assert z<height*10/2
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
/gps/position 0 {10*width/2} {z} mm
/run/beamOn 500"""
                f.write(text)
            counter +=1



def foo(args):
    obj = subprocess.run(["build/OpNovice",'-width',str(args[0]),'-height',str(args[1]),'-m',f'build/macros/beamMuon_{args[3]}.mac'])
    with open(f'data/params_{args[3]}.json','w') as f:
        json.dump({'z':args[2]},f)
    return obj

#with Pool(4) as pool:
#    pool.map(foo, params)

for arg in params:
    foo(arg)