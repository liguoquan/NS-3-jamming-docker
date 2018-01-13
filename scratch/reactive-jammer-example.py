import ns.core
import ns.network
import ns.mobility
import ns.wifi
import ns.internet
import ns.energy
import ns.jamming

def main():
    verbose = false

    c = ns.network.NodeContainer()
    c.Create (5)
    networkNodes = ns.network.NodeContainer() 
    networkNodes.Add(c.Get(0))
    networkNodes.Add(c.Get(1))
    networkNodes.Add(c.Get(2))
    networkNodes.Add(c.Get(3))

    wifi = ns.wifi.WifiHelper()
    wifi.SetStandard(ns.wifi.WIFI_PHY_STANDARD_80211b)

    NslWifiPhyHelper wifiPhy = NslWifiPhyHelper::Default ()
    wifiPhy.Set ("NslRxGain", DoubleValue (-10))
    wifiPhy.Set ("NslTxGain", DoubleValue (offset + Prss))
    wifiPhy.Set ("NslCcaMode1Threshold", DoubleValue (0.0))

if __name__=='__main__':
    main()