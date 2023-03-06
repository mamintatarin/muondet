import subprocess
from multiprocessing import Pool
import numpy as np


step_width = 1 # cm
step_height = 10 # cm
step_z = 10 # mm
maxheight = 100 # cm
maxwidth = 5 # cm
minwidth = 0.5 # cm
minheight = 10 # cm
params = []


for width in np.arange(minwidth,maxwidth,step_width):
    for height in np.arange(minheight,maxheight,step_height):
        for z in np.arange(0,0.99*10*height/2, step_z):
            params.append((width,height,z))



def foo(args):
    return subprocess.run(["build/OpNovice","-m","optPhoton.mac",'-width',str(args[0]),'-height',str(args[1]),'-z',str(args[2])])
    print(args)

with Pool(5) as pool:
    pool.map(foo, params)

#for arg in params:
#    foo(arg)