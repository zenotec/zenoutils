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

// libc++ includes
#include <iostream>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes

#include "GetInterfaceCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: GetInterfaceCommand
//*****************************************************************************

GetInterfaceCommand::GetInterfaceCommand(int index_)
{
  this->IfIndex.SetValue(index_);
}

GetInterfaceCommand::GetInterfaceCommand(const std::string& name_)
{
  this->IfName.SetValue(name_);
  this->IfIndex.SetValue((uint32_t)if_nametoindex(name_.c_str()));
  if (!this->IfIndex())
  {
    ZLOG_ERR("Error retrieving interface index for: " + name_);
    return;
  }
}

GetInterfaceCommand::~GetInterfaceCommand()
{
}

void
GetInterfaceCommand::Display() const
{
  std::cout << "Interface: " << std::endl;
  std::cout << "\tPhy:   \t" << this->PhyIndex.GetValue() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex.GetValue() << std::endl;
  std::cout << "\tName:  \t" << this->IfName.GetValue() << std::endl;
  std::cout << "\tType:  \t" << this->IfType.GetString() << std::endl;
  std::cout << "\tMAC:   \t" << this->Mac.GetString() << std::endl;
  std::cout << "\tFreq:  \t" << this->Frequency.GetValue() << std::endl;
  std::cout << "\tSSID:  \t" << this->Ssid.GetValue() << std::endl;
}

bool
GetInterfaceCommand::Exec()
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
    return (false);
  }

  if (!this->_sock.SetHandler(this))
  {
    ZLOG_ERR("Error setting up message handlers");
    return (false);
  }

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_GET_INTERFACE);

  // Set interface index attribute
  if (!cmdmsg.PutAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Error setting ifindex attribute");
    return (false);
  }

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending get_interface netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for get_interface netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);
}

int
GetInterfaceCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
//  msg.DisplayAttributes();

  if (!msg.GetAttribute(&this->PhyIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->PhyIndex.Id()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->IfIndex))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfIndex.Id()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->IfName))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfName.Id()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->IfType))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->IfType.Id()));
    return(NL_SKIP);
  }

  if (!msg.GetAttribute(&this->Mac))
  {
    ZLOG_ERR("Missing attribute: " + zLog::IntStr(this->Mac.Id()));
    return (NL_SKIP);
  }

  if (msg.GetAttribute(&this->Ssid))
  {
    msg.GetAttribute(&this->Frequency);
    msg.GetAttribute(&this->ChannelType);
    msg.GetAttribute(&this->ChannelWidth);
  }

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
GetInterfaceCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing GetInterfaceCommand: (" + zLog::IntStr(this->IfIndex()) +
      std::string("): ") + this->IfName());
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}