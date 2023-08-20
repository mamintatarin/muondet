import subprocess
from multiprocessing import Pool
import numpy as np
import datetime


widths = [1,2,3,4,5] # cm
heights = [15,30,40,70,100] # cm
step_z = 10 # mm
params = []


for width in widths:
    for height in heights:
        for z in np.arange(0,10*height/2, step_z):
            params.append((width,height,z,datetime.datetime.utcnow()))
            with open(f'build/macros/beamMuon_{params[-1][3]}.mac','w') as f:
                text = f"/control/verbose 0
                        /tracking/verbose 0
                        /event/verbose 0
                        /run/initialize
                        /gps/verbose 0
                        /gps/particle mu+
                        /gps/ene/mono 1 GeV
                        /gps/direction 0 -1 0
                        /gps/position 0 10 {z} cm
                        /run/beamOn 500"
                f.write(text)



def foo(args):
    return subprocess.run(["build/OpNovice","-m","beamPhoton.mac",'-width',str(args[0]),'-height',str(args[1]),'-z',str(args[2])])

with Pool(4) as pool:
    pool.map(foo, params)

#for arg in params:
#    foo(arg)