
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/energy-module.h"
#include "ns3/jamming-module.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

NS_LOG_COMPONENT_DEFINE ("ConstantJammerExample");

using namespace ns3;

ofstream outfile;

/**
 * \brief Packet receiving sink.
 *
 * \param socket Pointer to socket.
 */
void ReceivePacket(Ptr<Socket> socket){
  Ptr<Packet> packet;
  Address from;
  while(packet = socket->RecvFrom(from)){
    if (packet->GetSize() > 0){
      InetSocketAddress iaddr = InetSocketAddress::ConvertFrom(from);
      NS_LOG_UNCOND("--\nReceived one packet! Socket: "<< iaddr.GetIpv4()
        << " port: " << iaddr.GetPort() << " at time = " <<
        Simulator::Now().GetSeconds() << "\n--");
      outfile << Simulator::Now().GetSeconds() << " ip " << iaddr.GetIpv4() << " port " << iaddr.GetPort() << "\n";
    }
  }
}

/**
 * \brief Traffic generator.
 *
 * \param socket Pointer to socket.
 * \param pktSize Packet size.
 * \param n Pointer to node.
 * \param pktCount Number of packets to generate.
 * \param pktInterval Packet sending interval.
 */
static void GenerateTraffic(Ptr<Socket> socket, uint32_t pktSize, Ptr<Node> n, uint32_t pktCount, Time pktInterval){
  if (pktCount > 0){
    socket->Send(Create<Packet>(pktSize));
    Simulator::Schedule(pktInterval, &GenerateTraffic, socket, pktSize, n, pktCount - 1, pktInterval);
  }else{
    socket->Close();
  }
}

/**
 * \brief Trace function for remaining energy at node.
 *
 * \param oldValue Old remaining energy value.
 * \param remainingEnergy New remaining energy value.
 */
void RemainingEnergy(double oldValue, double remainingEnergy){
  NS_LOG_UNCOND(Simulator::Now ().GetSeconds () << "s Current remaining energy = " << remainingEnergy << "J");
  //outfile << ""
}

/**
 * \brief Trace function for total energy consumption at node.
 *
 * \param oldValue Old total energy consumption value.
 * \param totalEnergy New total energy consumption value.
 */
void TotalEnergy(double oldValue, double totalEnergy){
  NS_LOG_UNCOND(Simulator::Now().GetSeconds() <<"s Total energy consumed by radio = " << totalEnergy << "J");
}

/**
 * \brief Trace function for node RSS.
 *
 * \param oldValue Old RSS value.
 * \param rss New RSS value.
 */
void NodeRss(double oldValue, double rss){
  NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "s Node RSS = " << rss << "W");
  outfile << Simulator::Now().GetSeconds() << " RSS " << rss << "\n";
}

/**
 * \brief Trace function for node PDR.
 *
 * \param oldValue Old PDR value.
 * \param pdr New PDR value.
 */
void NodePdr(double oldValue, double pdr){
  NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "s Node PDR = " << pdr);
  outfile << Simulator::Now().GetSeconds() << " PDR " << pdr << "\n";
}

/**
 * \brief Trace function for node RX throughput.
 *
 * \param oldValue Old RX throughput value.
 * \param rxThroughput New RX throughput value.
 */
void NodeThroughputRx(double oldValue, double rxThroughput){
  NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "s Node RX throughput = " << rxThroughput);
}

int main (int argc, char *argv[]){
  /*
  LogComponentEnable ("NslWifiPhy", LOG_LEVEL_DEBUG);
  LogComponentEnable ("EnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("BasicEnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("DeviceEnergyModel", LOG_LEVEL_DEBUG);
  LogComponentEnable ("WifiRadioEnergyModel", LOG_LEVEL_DEBUG);
  LogComponentEnable ("WirelessModuleUtility", LOG_LEVEL_DEBUG);
  LogComponentEnable ("JammerHelper", LOG_LEVEL_DEBUG);
  LogComponentEnable ("Jammer", LOG_LEVEL_DEBUG);
  LogComponentEnable ("ReactiveJammer", LOG_LEVEL_DEBUG);
  LogComponentEnable ("JammingMitigationHelper", LOG_LEVEL_DEBUG);
  LogComponentEnable ("JammingMitigation", LOG_LEVEL_DEBUG);
  LogComponentEnable ("MitigateByChannelHop", LOG_LEVEL_DEBUG);
  */

  outfile.open("constant.log",ios::trunc);

  std::string phyMode("DsssRate1Mbps");
  double Prss = -80;            // dBm
  uint32_t PpacketSize = 200;   // bytes
  bool verbose = false;

  // simulation parameters
  uint32_t numPackets = 10000;  // number of packets to send
  double interval = 0.1;          // seconds
  double startTime = 0.0;       // seconds
  double distanceToRx = 10.0;   // meters
  /*
   * This is a magic number used to set the transmit power, based on other
   * configuration.
   */
  double offset = 81;

  CommandLine cmd;
  cmd.AddValue("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue("Prss", "Intended primary RSS (dBm)", Prss);
  cmd.AddValue("PpacketSize", "size of application packet sent", PpacketSize);
  cmd.AddValue("numPackets", "Total number of packets to send", numPackets);
  cmd.AddValue("startTime", "Simulation start time", startTime);
  cmd.AddValue("distanceToRx", "X-Axis distance between nodes", distanceToRx);
  cmd.AddValue("verbose", "Turn on all device log components", verbose);
  cmd.Parse (argc, argv);

  // Convert to time object
  Time interPacketInterval = Seconds(interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  NodeContainer c;
  c.Create(3);     // create 2 nodes + 1 jammer
  NodeContainer networkNodes;
  networkNodes.Add(c.Get(0));
  networkNodes.Add(c.Get(1));

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if(verbose){
    wifi.EnableLogComponents();
  }
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);

  /** Wifi PHY **/
  /***************************************************************************/
  NslWifiPhyHelper wifiPhy = NslWifiPhyHelper::Default();
  wifiPhy.Set("NslRxGain", DoubleValue(-10));
  wifiPhy.Set("NslTxGain", DoubleValue(offset + Prss));
  wifiPhy.Set("NslCcaMode1Threshold", DoubleValue(0.0));
  /***************************************************************************/

  /** wifi channel **/
  NslWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel");
  // create wifi channel
  Ptr<NslWifiChannel> wifiChannelPtr = wifiChannel.Create();
  wifiPhy.SetChannel(wifiChannelPtr);

  /** MAC layer **/
  // Add a non-QoS upper MAC, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default();
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
      StringValue(phyMode), "ControlMode", StringValue(phyMode));
  // Set it to ad-hoc mode
  wifiMac.SetType("ns3::AdhocWifiMac");

  /** install PHY + MAC **/
  NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, networkNodes);
  // install MAC & PHY onto jammer
  NetDeviceContainer jammerNetdevice = wifi.Install(wifiPhy, wifiMac, c.Get(2));

  /** mobility **/
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add(Vector(0.0, 0.0, 0.0));
  positionAlloc->Add(Vector(10, 0, 0.0));
  positionAlloc->Add(Vector(5, 3, 0.0)); // jammer location
  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(c);

  /** Energy Model **/
  /***************************************************************************/
  /* energy source */
  BasicEnergySourceHelper basicSourceHelper;
  // configure energy source
  basicSourceHelper.Set("BasicEnergySourceInitialEnergyJ", DoubleValue(0.1));
  // install on node
  EnergySourceContainer energySources = basicSourceHelper.Install(c);
  /* device energy model */
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set("TxCurrentA", DoubleValue(0.0174));
  // install on devices
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install(devices, energySources);
  DeviceEnergyModelContainer jammerDeviceModels = radioEnergyHelper.Install(jammerNetdevice.Get(0), energySources.Get(2));
  /***************************************************************************/

  /** WirelessModuleUtility **/
  /***************************************************************************/
  WirelessModuleUtilityHelper utilityHelper;
  // set inclusion/exclusion list for all nodes
  std::vector<std::string> AllInclusionList;
  AllInclusionList.push_back("ns3::UdpHeader");          // record only UdpHeader
  std::vector<std::string> AllExclusionList;
  AllExclusionList.push_back("ns3::olsr::PacketHeader"); // ignore all olsr headers/trailers
  // assign lists to helper
  utilityHelper.SetInclusionList(AllInclusionList);
  utilityHelper.SetExclusionList(AllExclusionList);
  // install on all nodes
  WirelessModuleUtilityContainer utilities = utilityHelper.InstallAll();
  /***************************************************************************/

  /** Jammer **/
  /***************************************************************************/
  JammerHelper jammerHelper;
  // configure jammer type
  //jammerHelper.SetJammerType("ns3:ConstantJammer");
  // set jammer parameters
  // jammerHelper.Set("ConstantJammerRxTimeout", TimeValue(Seconds(2.0)));
  // jammerHelper.Set("ConstantJammerReactionStrategy", UintegerValue(ConstantJammer::FIXED_PROBABILITY));
  // enable jammer reaction to jamming mitigation
  // jammerHelper.Set("ConstantJammerReactToMitigation", UintegerValue(true));
  // install jammer
  JammerContainer jammers = jammerHelper.Install(c.Get(2));
  // Get pointer to Jammer
  Ptr<Jammer> jammerPtr = jammers.Get(0);
  // enable all jammer debug statements
  if(verbose){
    jammerHelper.EnableLogComponents ();
  }
  /***************************************************************************/

  /** Internet stack **/
  InternetStackHelper internet;
  internet.Install(networkNodes);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO("Assign IP Addresses.");
  ipv4.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket(networkNodes.Get(1), tid);  // node 1, receiver
  InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), 80);
  recvSink->Bind(local);
  recvSink->SetRecvCallback(MakeCallback(&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket(networkNodes.Get(0), tid);    // node 0, sender
  InetSocketAddress remote = InetSocketAddress(Ipv4Address::GetBroadcast(), 80);
  source->SetAllowBroadcast(true);
  source->Connect(remote);

  /** connect trace sources **/
  /***************************************************************************/
  // all sources are connected to node 2
  // energy source
  //Ptr<EnergySource> basicSourcePtr = energySources.Get(1);
  //basicSourcePtr->TraceConnectWithoutContext("RemainingEnergy",MakeCallback(&RemainingEnergy));
  // using honest node device energy model list
  //Ptr<DeviceEnergyModel> basicRadioModelPtr = deviceModels.Get(1);
  //basicRadioModelPtr->TraceConnectWithoutContext("TotalEnergyConsumption",MakeCallback (&TotalEnergy));
  // wireless module utility
  Ptr<WirelessModuleUtility> utilityPtr = utilities.Get(1);
  utilityPtr->TraceConnectWithoutContext("Rss", MakeCallback(&NodeRss));
  utilityPtr->TraceConnectWithoutContext("Pdr", MakeCallback(&NodePdr));
  /***************************************************************************/


  /** simulation setup **/
  // start traffic
  Simulator::Schedule(Seconds(startTime), &GenerateTraffic, source,
                       PpacketSize, networkNodes.Get (0), numPackets,
                       interPacketInterval);

  // start jammer at 7.0 seconds
  Simulator::Schedule(Seconds(startTime), &ns3::Jammer::StartJammer,
                       jammerPtr);

  Simulator::Stop(Seconds(60.0));
  Simulator::Run();
  Simulator::Destroy();
  
  outfile.close();

  return 0;
}
