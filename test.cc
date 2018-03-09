/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include "ns3/mac-low.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");
EventId m_Event;
void func1(double ag)
{
  std::cout<<"ag="<<ag<<"\n";
}
void myfunc()
{

   // m_Event.Cancel ();
  //double ag=8;
  //Time time = 1;
   // NS_ASSERT (m_Event.IsRunning ());
  //  m_Event = Simulator::Schedule( time,&func1, this,ag);
    NS_LOG_UNCOND ("myfunc");
}

int 
main (int argc, char *argv[])
{
    //////jkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
MacLow maclow;
//maclow.SetSifs (10000);
  NS_LOG_UNCOND (maclow.GetSifs ());
  myfunc ();
  Simulator::Run ();
  Simulator::Destroy ();
}
