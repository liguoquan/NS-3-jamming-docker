import matplotlib.pyplot as plt 

color = ["r","g","b","y"]

def PDR_RSSI(PDR,RSSI,label):
    c = [color[d] for d in label]
    plt.xlabel('PDR')
    plt.ylabel('RSSI')
    plt.scatter(PDR,RSSI,c = c,marker = 'o') 
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
    plt.boxplot(RSSI,labels=range(len(RSSI)))
    plt.show()


if __name__=='__main__':
    PDR_RSSI(range(10),range(10),[i%2 for i in range(10)])
    RSSI_time(range(10),range(10))
    PDR_box([[x for x in range(10)] for j in range(2)])
    RSSI_box([[x for x in range(10)] for j in range(2)])