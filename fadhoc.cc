#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/stats-module.h"
#include <fstream>
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("coop-wifi");
bool verbose = true;

bool
SetVerbose (std::string value)
{
  verbose = true;
  return true;
}
//kklklkllkkkl
class CoopRtsHeader : public WifiMacHeader
{
public:
  CoopRtsHeader ();
  virtual ~CoopRtsHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator Headerstart);
  virtual uint32_t GetSerializedSize (void) const;
  void SetHelperId (Ipv4Address helperadd);
  void SetRSourceHelper (double rsh);
  void SetRHelperDestination (double rhd);
  Ipv4Address GetHelperId (void) const;
  double GetRSourceHelper (void) const;
  double GetRHelperDestination (void) const;


private:

  Ipv4Address m_helperid;
  double m_rsh;
  double m_rhd;

};
CoopRtsHeader::CoopRtsHeader ()
{
  m_rsh=0;
  m_rhd=0;
}
CoopRtsHeader::~CoopRtsHeader ()
{}
TypeId
CoopRtsHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CoopRtsHeader")
  .SetParent<WifiMacHeader> ()
  .AddConstructor<CoopRtsHeader> ()
  ;
  return tid;
}
TypeId
CoopRtsHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
CoopRtsHeader::Print (std::ostream &os) const
{
  os <<"helper="<< m_helperid
  <<" Rsh="<<(double) m_rsh
  <<" Rhd="<< (double)m_rhd;
}

uint32_t
CoopRtsHeader::GetSerializedSize (void) const
{
  return 20 ;
}

void
CoopRtsHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (m_helperid.Get());
  i.WriteU64 (m_rsh);
  i.WriteU8 (m_rhd);

}
uint32_t
CoopRtsHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_helperid.Set (i.ReadNtohU32 ());

  m_rsh = i.ReadU8 ();
  m_rhd = i.ReadU8 ();


// we return the number of bytes effectively read.
  return GetSerializedSize ();
}
void
CoopRtsHeader::SetHelperId (Ipv4Address helperid)
{
  m_helperid = helperid;
}
void
CoopRtsHeader::SetRHelperDestination (double rhd)
{
  m_rhd = rhd;
}
void
CoopRtsHeader::SetRSourceHelper (double rsh)
{
  m_rsh = rsh;
}

Ipv4Address
CoopRtsHeader::GetHelperId () const
{
  return m_helperid;
}
double
CoopRtsHeader::GetRHelperDestination () const
{
  return m_rhd;
}
double
CoopRtsHeader::GetRSourceHelper () const
{
  return m_rsh;
}

void
PrintCoopTable (std::list<MacLow::CoopTable> cooptable)
{
  std::list<MacLow::CoopTable>::iterator listIterator;

  for (listIterator=cooptable.begin(); listIterator!=cooptable.end(); listIterator++)
    {
     std::cout <<"\nSourceMacAddres "<<listIterator->SourceMacAddres;
     std::cout << "\nHelper-Destination DataRate=  " <<listIterator->Rhd<<"Mbs  ";
     std::cout << "\nSource-Helper DataRate=       " <<listIterator->Rsh<<"Mbs  ";
     std::cout << "\nSource-Destination DataRate=  " <<listIterator->Rsd<<"Mbs"<<std::endl;
     std::cout <<"time "<<listIterator->TimeOffHear;
     std::cout << "\n Helper Address=       " <<listIterator->HelperMacAddres<<std::endl;
    }
}



  int main (int argc, char *argv[])
    {
      CommandLine cmd;
      cmd.AddValue ("v", "Verbose (turns on logging).", MakeCallback (&SetVerbose));
      cmd.Parse (argc, argv);
      if (verbose)
        {
        // LogComponentEnable ("Node", LOG_LEVEL_FUNCTION);
       //  LogComponentEnable ("OnOffApplication", LOG_LEVEL_FUNCTION);
       //  LogComponentEnable ("WifiRemoteStationManager", LOG_LEVEL_FUNCTION);
       //  LogComponentEnable ("WifiNetDevice", LOG_LEVEL_FUNCTION);
       //  LogComponentEnable ("WifiMac", LOG_LEVEL_FUNCTION);
      //   LogComponentEnable ("DcaTxop", LOG_LEVEL_FUNCTION);
        // LogComponentEnable ("WifiRemoteStationManager",LOG_LEVEL_ALL);
       //  LogComponentEnable ("YansWifiPhy", LOG_LEVEL_FUNCTION);

        }
      NS_LOG_INFO ("Create nodes.");
      uint32_t nWifi = 6;
      double simulationTime = 20; //seconds
      uint8_t channelWidth = 20; //MHz

      NodeContainer wifiNodes;
      wifiNodes.Create (nWifi);



      YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
      YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
      wifiPhy.SetChannel (channel.Create ());


      /*//////////////////////////////////////////////////////
   //   phy.Set ("ShortGuardEnabled", BooleanValue (true));
      phy.Set ("Antennas", UintegerValue (3));
      phy.Set ("MaxSupportedTxSpatialStreams", UintegerValue (3));
      phy.Set ("MaxSupportedRxSpatialStreams", UintegerValue (3));
      //////////////////////////////////////////////////*/

      WifiMacHelper wifiMac;
      WifiHelper wifi;
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
     // wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
      //wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("HtMcs7"), "ControlMode", StringValue("HtMcs0"));
     wifi.SetRemoteStationManager ("ns3::IdealWifiManager");//CaraWifiManager   ArfWifiManager   IdealWifiManager
    // wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue ("HtMcs7"));
       Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (0));

      // wifi.EnableLogComponents();
       Ssid ssid = Ssid ("ns3-80211n");
       wifiMac.SetType ("ns3::AdhocWifiMac","BE_BlockAckThreshold",UintegerValue (0));



         NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, wifiNodes);



       // Set channel width
       Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (channelWidth));

       // mobility
      MobilityHelper mobility;
      Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
      positionAlloc->Add (Vector (0.0, 0.0, 0.0));
      positionAlloc->Add (Vector (45, 0, 0.0));
      positionAlloc->Add (Vector (90,0, 0.0));
      positionAlloc->Add (Vector (180.0,0.0, 0.0));
      positionAlloc->Add (Vector (180.0,0.0, 0.0));
      positionAlloc->Add (Vector (180.0,0.0, 0.0));

   /*   for (uint32_t i = 0; i < nWifi; i++)
     //   {
     //     positionAlloc->Add (Vector (distance, 0.0, 0.0));
       }*/
      mobility.SetPositionAllocator (positionAlloc);
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (wifiNodes.Get(0));
      mobility.Install (wifiNodes.Get(1));
      mobility.Install (wifiNodes.Get(2));
      mobility.Install (wifiNodes.Get(3));
      mobility.Install (wifiNodes.Get(4));
      mobility.Install (wifiNodes.Get(5));


      // Internet stack
      InternetStackHelper stack;
      stack.Install (wifiNodes);

      Ipv4AddressHelper address;

      address.SetBase ("192.168.1.0", "255.255.255.0");
      Ipv4InterfaceContainer staNodeInterfaces;
      staNodeInterfaces = address.Assign (devices);

      NodeContainer const & n = NodeContainer::GetGlobal ();
        for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
          {
            Ptr<Node> node = *i;
            Ptr<MobilityModel> mob = node->GetObject<MobilityModel> ();
            if (! mob) continue; // Strange -- node has no mobility model       installed. Skip.
            Ptr<NetDevice> PtrNetDevice = node->GetDevice(0);
            Vector pos = mob->GetPosition ();
            Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> (); // Get Ipv4 instance of the node
            Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
            Ipv4Address addri = iaddr.GetLocal ();
            std::cout <<"mac address "<<PtrNetDevice->GetAddress() << " IP: " << addri<< " is at (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
          }


        // receiver---

      PacketSinkHelper sinkHelper1 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
      ApplicationContainer sinkApp = sinkHelper1.Install (wifiNodes.Get(0));
      sinkApp.Start (Seconds (0.0));
      sinkApp.Stop (Seconds (simulationTime+1));


        // Sender ----
      OnOffHelper Sender("ns3::UdpSocketFactory", (InetSocketAddress (staNodeInterfaces.GetAddress (0), 9)));
      ApplicationContainer SenderApp;
      for (uint32_t i = 1; i < nWifi; ++i)
        {
           Sender.SetConstantRate( DataRate("65Kbps") , 2000 );
           Sender.SetAttribute("MaxBytes", UintegerValue (2000));
          // Sender.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
           //Sender.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
           //Sender.SetAttribute ("OnTime", StringValue ("1"));
           //Sender.SetAttribute ("OffTime", StringValue ("0"));
           SenderApp = Sender.Install(wifiNodes.Get(i));

        }
      SenderApp.Start (Seconds (1.0));
      SenderApp.Stop (Seconds (simulationTime ));


   // Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  //pcab
      wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
      wifiPhy.EnablePcap ("coopmacsource1", devices.Get(1));
    //  Config::Connect ("/NodeList/*/DeviceList/*/Mac/AMPDUSIZE", MakeCallback (&DevAmpduSizeTrace));
  //    Config::Connect ("/NodeList/*/DeviceList/*/Mac/CoopTable", MakeCallback(&PrintCoopTable));

      CoopRtsHeader cooprtsheader;

      Simulator::Stop (Seconds (simulationTime + 1));
      Simulator::Run ();
      Simulator::Destroy ();
      return 0;
   }
