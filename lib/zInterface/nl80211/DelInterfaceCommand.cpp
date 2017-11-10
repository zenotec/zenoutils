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

#include "IfIndexAttribute.h"
#include "IfNameAttribute.h"
#include "DelInterfaceCommand.h"


namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: DelInterfaceCommand
//*****************************************************************************

DelInterfaceCommand::DelInterfaceCommand(int index_)
{
  this->IfIndex.SetValue(index_);
}

DelInterfaceCommand::DelInterfaceCommand(const std::string& name_)
{
  this->IfName.SetValue(name_);
  this->IfIndex.SetValue((uint32_t)if_nametoindex(name_.c_str()));
  if (!this->IfIndex())
  {
    ZLOG_ERR("Error retrieving interface index for: " + name_);
    return;
  }
}

DelInterfaceCommand::~DelInterfaceCommand()
{
}

bool
DelInterfaceCommand::Exec()
{
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

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_DEL_INTERFACE);
  cmdmsg.PutAttribute(&this->IfIndex);
  this->_sock.SendMsg(cmdmsg);
  this->_sock.RecvMsg();
  this->_sock.Disconnect();

  return(true);
}

void
DelInterfaceCommand::Display() const
{
  std::cout << "Deleted Interface: " << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex.GetValue() << std::endl;
  std::cout << "\tName:  \t" << this->IfName.GetValue() << std::endl;
}

int
DelInterfaceCommand::valid_cb(struct nl_msg* msg_, void* arg_)
{

  GenericMessage msg(msg_);
  if (!msg.Parse())
  {
    ZLOG_ERR("Error parsing generic message");
    return (NL_SKIP);
  }
  msg.DisplayAttributes();

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

  return(NL_OK);
}

int
DelInterfaceCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  return (NL_SKIP);
}

}