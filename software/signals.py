import numpy as np
import matplotlib.pyplot as plt
import os
import sys

def initFile(fileName): #odczytanie danych z pliku i stworzenie obiektów danych
    path_to_data=os.path.join(sys.path[0], fileName)
    file=open(path_to_data)
    dataArray=[]
    temperature=[]
    accelerometer=[]
    moisture=[]
    tensometer=[]
    time=[]
    timeValue=0
    file_text=file.read()
    file_lines=file_text.split("\n")
    for i in range(len(file_lines)-1):
        file_line=file_lines[i]
        line_array=file_line.split(" ")
        line_array.pop(0)
        if line_array[0]=="Temperatura:":
            temperature.append((float(line_array[1])-25)*8)
        if line_array[0]=="Wilgotnosc:":
            moisture.append(float(line_array[1]))
        if line_array[0]=="Grawitacja:":
            # print(line_array)
            if line_array[1]=="-z":
                accelerometer.append(50)
            else:
                accelerometer.append(0)
        if line_array[0]=="Lezy":
            print("chuj")
            tensometer.append(50)
        if line_array[0]=="Nie" and line_array[1]=="lezy":
            tensometer.append(0)
        if line_array[0]=="Wstaje":
            timeValue=timeValue+1
            time.append(timeValue)
            if len(accelerometer)<(len(time)-1):
                accelerometer.append(0)
            if len(tensometer)<(len(time)-1):
                tensometer.append(0)
        
        
    file.close()
    return temperature, moisture, accelerometer, tensometer, time

temperature, moisture, accelerometer, tensometer, time=initFile("data1.txt")

print(len(temperature), len(moisture), len(accelerometer), len(tensometer), len(time))

# print(temperature, moisture, accelerometer, tensometer, time)

plt.plot(time,temperature, '.-')
plt.plot(time,accelerometer, '-')
plt.plot(time,moisture, '.-')
plt.plot(time,tensometer, '-')
plt.grid()
plt.show()
