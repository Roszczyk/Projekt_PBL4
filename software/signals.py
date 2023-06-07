import numpy as np
import matplotlib.pyplot as plt

time=[]
temperature=[]
accelerometer=[]
moisture=[]
rssi=[]
tensometer=[]

for i in range(100):
    time.append(i)
    tensometer.append(np.random.choice([0,1])*50)
    temperature.append(np.random.choice([21,22,23,24,25,26,27,28]))
    accelerometer.append(np.random.choice([0,1])*50)
    rssi.append(np.random.choice([-63,-64,-65,-66,-67])+100)
    moisture.append(np.random.choice([40,50,60,70,45,55,65]))

plt.plot(time,temperature, '.-')
plt.plot(time,accelerometer, '-')
plt.plot(time,moisture, '.-')
plt.plot(time,rssi, '.-')
plt.plot(time,tensometer, '-')
plt.grid()
plt.show()
