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

// libc++ includes
#include <iostream>
#include <map>

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;

// local includes
#include "Attribute.h"
#include "Command.h"

#include "Message.h"
#include "Handler.h"
#include "Socket.h"
#include "RouteSocket.h"
using namespace netlink;

#include "GetLinkCommand.h"


namespace netlink
{

static std::string
__errstr(int code)
{
  return(std::string(nl_geterror(code)));
}

//*****************************************************************************
// Class: GetLinkCommand
//*****************************************************************************

GetLinkCommand::GetLinkCommand(int index_)
{
  this->Link.IfIndex(index_);
}

GetLinkCommand::GetLinkCommand(const std::string& name_)
{
  this->Link.IfName(name_);
}

GetLinkCommand::~GetLinkCommand()
{
  this->_sock.Disconnect();
}

bool
GetLinkCommand::Exec()
{
  int ret = 0;
  struct rtnl_link* link = NULL;

  if (!this->Link.IfIndex() && this->Link.IfName().empty())
  {
    ZLOG_ERR("Either valid ifindex or name must be specified");
    return(false);
  }

  if (!this->_sock.Connect())
  {
    ZLOG_ERR("Error connecting Netlink socket");
    return(false);
  }

  int index = this->Link.IfIndex();
  const char* name = (this->Link.IfName().empty() ? NULL : this->Link.IfName().c_str());

  ret = rtnl_link_get_kernel(this->_sock(), index, name, &link);
  if (ret < 0)
  {
    ZLOG_ERR("Error executing GetLinkCommand");
    ZLOG_ERR("Error: (" + ZLOG_INT(ret) + ") " + __errstr(ret));
    return(false);
  }

  this->Link(link);
  this->_sock.Disconnect();

  return(true);
}

void
GetLinkCommand::Display() const
{
  this->Link.Display();
}

}
