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
#include <zutils/nl80211/NewBeaconCommand.h>

// local includes

ZLOG_MODULE_INIT(zUtils::zLog::Log::MODULE_WIRELESS);

namespace nl80211
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: NewBeaconCommand
//*****************************************************************************


NewBeaconCommand::NewBeaconCommand(const unsigned int ifindex_) :
    Command(ifindex_)
{
  this->IfIndex(ifindex_);
}

NewBeaconCommand::~NewBeaconCommand()
{
}

bool
NewBeaconCommand::Exec()
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

  GenericMessage cmdmsg(this->_sock.Family(), 0, NL80211_CMD_NEW_BEACON);
  cmdmsg.PutAttribute(&this->IfIndex);
  cmdmsg.PutAttribute(&this->BeaconInterval);
  cmdmsg.PutAttribute(&this->DtimPeriod);
  cmdmsg.PutAttribute(&this->BeaconHead);
  cmdmsg.PutAttribute(&this->BeaconTail);

  // Send message
  if (!this->_sock.SendMsg(cmdmsg))
  {
    ZLOG_ERR("Error sending get_interface netlink message");
    return(false);
  }

  // Wait for the response
  if (!this->_sock.RecvMsg())
  {
    ZLOG_ERR("Error receiving response for new_beacon netlink message");
    return(false);
  }

  if (!this->_count.TimedWait(100))
  {
    ZLOG_ERR("Error receiving response for new_beacon netlink message");
    return(false);
  }

  // Clean up
  this->_sock.Disconnect();

  return(this->_status);

}

void
NewBeaconCommand::Display() const
{
  std::cout << "##################################################" << std::endl;
  std::cout << "NewBeaconCommand: " << std::endl;
  std::cout << "\tName:  \t" << this->IfName.GetValue() << std::endl;
  std::cout << "\tIndex: \t" << this->IfIndex.GetValue() << std::endl;
  std::cout << "\tSsid:  \t" << this->Ssid.GetString() << std::endl;
  std::cout << "\tBINT:  \t" << this->BeaconInterval.GetValue() << std::endl;
  std::cout << "\tDTIM:  \t" << this->DtimPeriod.GetValue() << std::endl;
  std::cout << "\tBHEAD: \t" << this->BeaconHead.GetValue().second << std::endl;
  std::cout << "\tBTAIL: \t" << this->BeaconTail.GetValue().second << std::endl;
  std::cout << "##################################################" << std::endl;
}

int
NewBeaconCommand::valid_cb(struct nl_msg* msg_, void* arg_)
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

  this->_status = true;
  this->_count.Post();

  return (NL_OK);
}

int
NewBeaconCommand::err_cb(struct sockaddr_nl* nla, struct nlmsgerr* nlerr, void* arg)
{
  ZLOG_ERR("Error executing NewBeaconCommand");
  ZLOG_ERR("Error: (" + ZLOG_INT(nlerr->error) + ") " + __errstr(nlerr->error));
  this->_status = false;
  this->_count.Post();
  return(NL_SKIP);
}

}
