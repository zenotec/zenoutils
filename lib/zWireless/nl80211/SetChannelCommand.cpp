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

#include "SetChannelCommand.h"

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: SetChannelCommand
//*****************************************************************************

SetChannelCommand::SetChannelCommand(int index_) :
    Command(index_)
{
  this->IfIndex.SetValue(index_);
}

SetChannelCommand::SetChannelCommand(const std::string& name_) :
    Command(name_)
{
  this->IfName.SetValue(name_);
}

SetChannelCommand::~SetChannelCommand()
{
}

bool
SetChannelCommand::Exec()
{

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

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_SET_CHANNEL);
  cmdmsg.PutAttribute(&this->IfIndex);
  cmdmsg.PutAttribute(&this->Frequency);
  cmdmsg.PutAttribute(&this->ChannelWidth);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending set_channel netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for set_channel netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for set_channel netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);
}

void
SetChannelCommand::Display() const
{
  std::cout << "Set Channel: " << std::endl;
  std::cout << "\tName:  \t" << this->IfName.GetValue() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex.GetValue() << std::endl;
  std::cout << "\tChannel:  \t" << this->Frequency.GetChannel() << std::endl;
}

int
SetChannelCommand::ack_cb(struct nl_msg* msg_, void* arg_)
{

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
SetChannelCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing SetChannelCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}