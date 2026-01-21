import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from typing import Any

names=['x','y','h','a','b','cos','atmos','trunc','shad']
vmins=[None,4.75,4.75,None,None,None,None,]
vmaxs=[None,4.76,4.76,None,None,None,None,]

df=pd.read_csv("tmp.csv",names=names)
print(df.tail())

x:Any=df['x'].values
y:Any=df['y'].values

for nm,vmin,vmax in zip(names[2:],vmins,vmaxs):

    fig,ax=plt.subplots()
    sc = ax.scatter(x,y,c=df[nm],cmap='viridis',s=10,alpha=0.7,vmin=vmin,vmax=vmax)
    plt.colorbar(sc,ax=ax,label=nm)
    fig.savefig(f'img/result_{nm}.png',dpi=200)
# plt.show()