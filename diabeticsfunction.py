import matplotlib.pyplot as plt
import numpy as np

data=[0]*100
time=[0]*100

clock=0

def adddata(newdata):
    data.append(newdata)
    data.pop(0)
    time.append(clock)
    time.pop(0)

def det(x,y):
    detfun=[0]
    for i in range(len(x)-1):
        detfun.append((y[i+1]-y[i])/(x[i+1]-x[i]))
    return detfun

for i in range(150):
    clock=clock+1
    adddata(np.random.choice(np.linspace(-10,10,100)))

print(time)

ddata= det(time, data)

plt.plot(time, data, '-')
plt.plot(time, ddata,'-')
plt.grid()
plt.show()
