/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// libc includes
#include <stdlib.h>
#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Command.h"
#include "Attribute.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "GenericMessage.h"
#include "GenericSocket.h"
using namespace netlink;

#include "StartApCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: StartApCommand
//*****************************************************************************

StartApCommand::StartApCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(ifindex_);
}

StartApCommand::~StartApCommand()
{
}

bool
StartApCommand::Exec()
{

  this->_status = false;
  this->_count.Reset();

  if (!this->IfIndex())
  {
    ZLOG_ERR("Error getting interface index for: " + this->IfName());
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting NL80211 socket");
    return(false);
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return(false);
  }

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_START_AP);

  if (!cmdmsg.PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error putting interface index attribute");
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->Ssid))
  {
    ZLOG_ERR("Error putting SSID attribute: " + this->Ssid.GetString());
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->Channel))
  {
    ZLOG_ERR("Error putting channel attribute: " + zToStr(this->Channel.GetChannel()));
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->BeaconInterval))
  {
    ZLOG_ERR("Error putting beacon interval attribute: " + zToStr(this->BeaconInterval.GetValue()));
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->DtimPeriod))
  {
    ZLOG_ERR("Error putting DTIM period attribute");
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->BeaconHead))
  {
    ZLOG_ERR("Error putting beacon head attribute");
    return (false);
  }

  if (!cmdmsg.PutAttribute(&this->BeaconTail))
  {
    ZLOG_ERR("Error putting beacon tail attribute");
    return (false);
  }

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(1000))
  {
    ZLOG_ERR("Error receiving response for netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);
}

void
StartApCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "StartApCommand: " << std::endl;
  if (this->IfName.IsValid())
    std::cout << "\tName:   \t" << this->IfName.GetValue() << std::endl;
  if (this->IfIndex.IsValid())
    std::cout << "\tIndex:  \t" << this->IfIndex.GetValue() << std::endl;
  if (this->Ssid.IsValid())
    std::cout << "\tSsid:   \t" << this->Ssid.GetString() << std::endl;
  if (this->Channel.IsValid())
    std::cout << "\tChannel:\t" << this->Channel.GetChannel() << std::endl;
  if (this->BeaconInterval.IsValid())
    std::cout << "\tBINT:   \t" << this->BeaconInterval.GetValue() << std::endl;
  if (this->DtimPeriod.IsValid())
    std::cout << "\tDTIM:   \t" << this->DtimPeriod.GetValue() << std::endl;
  if (this->BeaconHead.IsValid())
    std::cout << "\tBHEAD:  \t" << this->BeaconHead.GetValue().second << std::endl;
  if (this->BeaconTail.IsValid())
    std::cout << "\tBTAIL:  \t" << this->BeaconTail.GetValue().second << std::endl;
  std::cout << "##################################################" << std::endl;
}

int
StartApCommand::ack_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }

//  std::cout << "StartApCommand::ack_cb()" << std::endl;
//  msg.Display();
//  msg.DisplayAttributes();

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
StartApCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing StartApCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
