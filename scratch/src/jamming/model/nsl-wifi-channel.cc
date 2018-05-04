/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/object-factory.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "nsl-wifi-channel.h"
#include "nsl-wifi-phy.h"

NS_LOG_COMPONENT_DEFINE ("NslWifiChannel");

namespace ns3 {

TypeId
NslWifiChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NslWifiChannel")
    .SetParent<WifiChannel> ()
    .AddConstructor<NslWifiChannel> ()
    .AddAttribute ("PropagationLossModel", "A pointer to the propagation loss model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&NslWifiChannel::m_loss),
                   MakePointerChecker<PropagationLossModel> ())
    .AddAttribute ("PropagationDelayModel", "A pointer to the propagation delay model attached to this channel.",
                   PointerValue (),
                   MakePointerAccessor (&NslWifiChannel::m_delay),
                   MakePointerChecker<PropagationDelayModel> ())
    ;
  return tid;
}

NslWifiChannel::NslWifiChannel ()
{}
NslWifiChannel::~NslWifiChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_phyList.clear ();
}

void
NslWifiChannel::SetPropagationLossModel (Ptr<PropagationLossModel> loss)
{
  m_loss = loss;
}

void
NslWifiChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  m_delay = delay;
}

void
NslWifiChannel::Send (Ptr<NslWifiPhy> sender, Ptr<const Packet> packet, double txPowerDbm,
                       WifiMode wifiMode, WifiPreamble preamble) const
{
  Ptr<MobilityModel> senderMobility = sender->GetMobility ()->GetObject<MobilityModel> ();
  NS_ASSERT (senderMobility != 0);
  uint32_t j = 0;
  for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++, j++)
    {
      if (sender != (*i))
        {
          // For now don't account for inter channel interference
          if ((*i)->GetChannelNumber() != sender->GetChannelNumber())
              continue;

          Ptr<MobilityModel> receiverMobility = (*i)->GetMobility ()->GetObject<MobilityModel> ();
          Time delay = m_delay->GetDelay (senderMobility, receiverMobility);
          double rxPowerDbm = m_loss->CalcRxPower (txPowerDbm, senderMobility, receiverMobility);
          NS_LOG_DEBUG ("propagation: txPower="<<txPowerDbm<<"dbm, rxPower="<<rxPowerDbm<<"dbm, "<<
                        "distance="<<senderMobility->GetDistanceFrom (receiverMobility)<<"m, delay="<<delay);
          Ptr<Packet> copy = packet->Copy ();
          Ptr<Object> dstNetDevice = m_phyList[j]->GetDevice ();
          uint32_t dstNode;
          if (dstNetDevice == 0)
            {
              dstNode = 0xffffffff;
            }
          else
            {
              dstNode = dstNetDevice->GetObject<NetDevice> ()->GetNode ()->GetId ();
            }
          Simulator::ScheduleWithContext (dstNode,
                                          delay, &NslWifiChannel::Receive, this,
                                          j, copy, rxPowerDbm, wifiMode, preamble);
        }
    }
}

void
NslWifiChannel::Receive (uint32_t i, Ptr<Packet> packet, double rxPowerDbm,
                          WifiMode txMode, WifiPreamble preamble) const
{
  m_phyList[i]->StartReceivePacket (packet, rxPowerDbm, txMode, preamble);
}

uint32_t
NslWifiChannel::GetNDevices (void) const
{
  return m_phyList.size ();
}
Ptr<NetDevice>
NslWifiChannel::GetDevice (uint32_t i) const
{
  return m_phyList[i]->GetDevice ()->GetObject<NetDevice> ();
}

void
NslWifiChannel::Add (Ptr<NslWifiPhy> phy)
{
  m_phyList.push_back (phy);
}

} // namespace ns3
