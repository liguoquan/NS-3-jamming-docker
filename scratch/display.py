from util import *
from math import log10

PDR = []
RSSI = []
for distance in range(1,1500,100):
    for interval in ["0.01","0.1","0.2","0.5","0.8","1"]:
        name = "data/none-"+str(distance)+"-"+interval+".log"
        with open(name,"rb") as f:
            a = f.readlines()[-3:]
            PDR.append(float(a[0][13:-1]))
            b = float(a[1][13:-1])
            b = log10(b*1000)*10
            RSSI.append(b)
PDR_RSSI(PDR,RSSI,[0 for i in range(len(PDR))])
