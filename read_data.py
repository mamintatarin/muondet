import numpy as np
import matplotlib.pyplot as plt
from collections import Counter
import os
import json

dtype = dtype = [('width',float),('height',float),('energy1',float),('energy2',float),('z',float)]
data = []
for i in range(69):
    tempdata=np.fromfile(os.path.join('data', f'Muons{i}.bin'),dtype=dtype)
    with open(f'data/params_{i}.json','r') as f:
        tempdata['z'] = json.load(f)['z']
    data.append(tempdata)
    tempdata_copy = tempdata.copy()
    tempdata_copy['z']=-tempdata_copy['z']
    tempdata_copy['energy1']=tempdata['energy2']
    tempdata_copy['energy2']=tempdata['energy1']
    data.append(tempdata_copy)
    
data = np.concatenate(data)
print(data)

def get_data_by_params(data: np.ndarray,width:float, height:float):
    return data[(data['width']==width)&(data['height']==height)]



def get_mean_table(data: np.ndarray, method: str = 'average') -> tuple[np.ndarray,np.ndarray]:
    unique_coords = np.unique(data['z'])
    result = np.zeros((unique_coords.shape[0],2))
    for i,z in enumerate(unique_coords):
        if method=='average':
            result[i] = np.asarray([data[data['z']==z]['energy1'].mean(),data[data['z']==z]['energy2'].mean()])
        elif method=="median":
            result[i] = np.asarray([np.median(data[data['z']==z]['energy1']),np.median(data[data['z']==z]['energy2'])])
        elif method=='most probable':
            result[i] = np.asarray([get_most_prob(data[data['z']==z]['energy1']),get_most_prob(data[data['z']==z]['energy2'])])
        elif method=='expected value':
            result[i] = np.asarray([get_expected_value(data[data['z']==z]['energy1']),get_expected_value(data[data['z']==z]['energy2'])])
        else:
            raise ValueError('No such method')
    return result,unique_coords

def predict_coords(energs: np.ndarray, mean_table: np.ndarray, unique_coords: np.ndarray, named_array: bool = True):
    if named_array:
        energy1 = energs['energy1']
        energy2 = energs['energy2']
    else:
        energy1 = energs[:,0]
        energy2 = energs[:,1]
    diff = np.zeros((energs.shape[0],mean_table.shape[0]))
    for i in range(mean_table.shape[0]):
        diff[:,i] = np.sqrt((energy1-mean_table[i,0])**2 + (energy2-mean_table[i,1])**2)
    min_index = np.argmin(diff,axis=1)
    predicted_coords = unique_coords[min_index]
    best_matches = mean_table[min_index,:]
    return predicted_coords, best_matches
    
    
    

params_2d = np.zeros((data.shape[0],2))
params_2d[:,0]=data['width']
params_2d[:,1]=data['height']
unique_width_height_array = np.vstack(list({tuple(row) for row in params_2d}))
for width,height in unique_width_height_array:
    tempdata = get_data_by_params(data, width, height)
    mean_table_temp, coords_z = get_mean_table(tempdata,method='average')
    #print(mean_table_temp,coords_z)
    predicted_coords, best_matches = predict_coords(tempdata, mean_table_temp, coords_z)
    errors_dicts = [] 
    for i,z in enumerate(coords_z):
        errors = tempdata[tempdata['z']==z]['z'] - predicted_coords[tempdata['z']==z]
        errors_dicts.append({'mean':errors.mean(),"std":errors.std(),"z":z})
    print("width:",width,"height:",height,"sys mean:",np.mean([abs(x['mean']) for x in errors_dicts]),
          'sys max:',np.max([abs(x['mean']) for x in errors_dicts]),
          'stat mean',np.mean([x['std'] for x in errors_dicts]),'stat max:',np.max([x['std'] for x in errors_dicts]))
    #print(width,height,mean_table_temp[coords_z==0])
