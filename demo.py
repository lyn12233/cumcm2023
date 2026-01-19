import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from typing import Any

df=pd.read_csv("tmp.csv",names=['x','y','h','a','b','cos','atmos','trunc','shad'])
print(df.tail())

x:Any=df['x'].values
y:Any=df['y'].values
h:Any=df['h'].values

fig,ax=plt.subplots()
sc = ax.scatter(x,y,c=df['shad'],cmap='viridis',s=10,alpha=0.7)
plt.colorbar(sc,ax=ax,label='test')

plt.show()