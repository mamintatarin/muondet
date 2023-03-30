import subprocess
from multiprocessing import Pool
import numpy as np


widths = [1] # cm
heights = [30,40,70,100] # cm
step_z = 10 # mm
params = []


for width in widths:
    for height in heights:
        for z in np.arange(0,0.99*10*height/2, step_z):
            params.append((width,height,z))



def foo(args):
    return subprocess.run(["build/OpNovice","-m","optPhoton.mac",'-width',str(args[0]),'-height',str(args[1]),'-z',str(args[2])])

with Pool(4) as pool:
    pool.map(foo, params)

#for arg in params:
#    foo(arg)