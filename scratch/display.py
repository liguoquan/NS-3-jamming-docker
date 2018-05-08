from util import *
from math import log10

PDR = []
RSSI = []
label = []
for distance in range(1,12000,100):
    for interval in ["0.0005","0.001","0.005","0.01","0.1","0.5","1"]:
        name = "data/none/none-"+str(distance)+"-"+interval+".log"
        with open(name,"rb") as f:
            a = f.readlines()[-3:]
            PDR.append(float(a[0][13:-1]))
            b = float(a[1][13:-1])
            b = log10(b*1000)*10
            RSSI.append(b)
            label.append(0)
PDR_RSSI(PDR,RSSI,label)

"""
for disx in range(0,16,1):
    for interval in ["0.01","0.1","1"]:
        for disy in range(0,11,1):
            name = "data/constant/constant-"+str(disx)+"-"+str(disy)+"-"+interval+".log"
            with open(name,"rb") as f:
                a = f.readlines()[-3:]
                PDR.append(float(a[0][13:-1]))
                b = float(a[1][13:-1])
                b = log10(b*1000)*10
                RSSI.append(b)
                label.append(1)
PDR_RSSI(PDR,RSSI,label)
"""
for disx in range(0,151,5):
    for interval in ["0.01","0.1","1"]:
        for disy in range(0,101,5):
            a = "%.2f"%(disx/10)
            b = "%.2f"%(disy/10)
            name = "data/constant/constant-"+a+"-"+b+"-"+interval+".log"
            with open(name,"rb") as f:
                a = f.readlines()[-3:]
                PDR.append(float(a[0][13:-1]))
                b = float(a[1][13:-1])
                b = log10(b*1000)*10
                RSSI.append(b)
                label.append(1)
PDR_RSSI(PDR,RSSI,label)

for disx in range(0,16,1):
    for interval in ["0.01","0.1","1"]:
        for disy in range(0,11,1):
            name = "data/reactive/reactive-"+str(disx)+"-"+str(disy)+"-"+interval+".log"
            with open(name,"rb") as f:
                a = f.readlines()[-3:]
                PDR.append(float(a[0][13:-1]))
                b = float(a[1][13:-1])
                b = log10(b*1000)*10
                RSSI.append(b)
                label.append(2)
PDR_RSSI(PDR,RSSI,label)


for disx in range(0,16,1):
    for interval in ["0.01","0.1","1"]:
        for disy in range(0,11,1):
            name = "data/random/random-"+str(disx)+"-"+str(disy)+"-"+interval+".log"
            with open(name,"rb") as f:
                a = f.readlines()[-3:]
                PDR.append(float(a[0][13:-1]))
                b = float(a[1][13:-1])
                b = log10(b*1000)*10
                RSSI.append(b)
                label.append(3)
PDR_RSSI(PDR,RSSI,label)