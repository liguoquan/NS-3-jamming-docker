import matplotlib.pyplot as plt 

color = ["r","g","b","y"]

def PDR_RSSI(PDR,RSSI,label):
    c = [color[d] for d in label]
    plt.xlabel('PDR')
    plt.ylabel('RSSI')
    plt.scatter(x,y,c = c,marker = 'o') 
    plt.show()

def RSSI_time(t,RSSI):
    plt.xlabel('t')
    plt.ylabel('RSSI')
    plt.plot(t, RSSI,'r')
    plt.show() 

def PDR_box(PDR):
    plt.boxplot(PDR,labels=range(len(PDR)))
    plt.show()

def RSSI_box(RSSI):
    plt.boxplot(PDR,labels=range(len(RSSI)))
    plt.show()


if __name__=='__name__':
    PDR_RSSI(range(10),range(10),[i%2 for i in range(10)])
