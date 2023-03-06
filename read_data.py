import numpy as np
import matplotlib.pyplot as plt

dtype = [('z',float),('width',float),('height',float),('energy1',float),('energy2',float)]
data=np.fromfile('build/data/Muons.bin',dtype=dtype)
print(data)
print(data.shape)
print(data['energy1'].mean(),data['energy2'].mean())
sum_energs = data['energy1']+data['energy2']
print(sum_energs.mean())
plt.hist(sum_energs,bins=50)
plt.show()
plt.clf()
