
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation
import time


x_num=100
x=np.arange(x_num)
y=np.empty(x_num)
y[:]=np.NaN
fig=plt.figure(figsize=[15,7])
ax = plt.axes(xlim=(0,x_num),ylim=(325,575))
line,=ax.plot(x,y,lw=2)

def init():
    line.set_data([],[])
    return line,

def animate(i):
    global y
    global heart
    f = open("heart.txt", 'r')
    temp = f.readline()
    if(temp != ''):
        heart = int(temp)
        line.set_data(x,y)
    y[i]= heart
    if i==x_num-1:
        y[:]=np.NaN
        line.set_data(x,y)
    return line,

ani=animation.FuncAnimation(fig,animate,init_func=init,frames=x_num,interval=1)
plt.show()
