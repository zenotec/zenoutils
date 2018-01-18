/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
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

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <system_error>

#include <zutils/zLog.h>

namespace zUtils
{
namespace zLog
{

//*****************************************************************************
// Class: Manager
//*****************************************************************************

Manager::Manager() :
    _thread(this, this)
{
  this->_conn.resize(Log::LEVEL_LAST, NULL);
  this->_lock.Unlock();
  this->_thread.Start();
}

Manager::~Manager()
{
  this->_conn.clear();
  this->_thread.Stop();
  this->_lock.Lock();
}

bool
Manager::RegisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_max_level[module_] = Log::LEVEL_DEF;
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::RegisterModule(const Log::MODULE module_)
{
  return (this->RegisterModule(Log::ToString(module_)));
}

bool
Manager::UnregisterModule(const std::string& module_)
{
  bool status = false;
  if (this->_max_level.count(module_) && this->_lock.Lock())
  {
    this->_max_level.erase(module_);
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterModule(const Log::MODULE module_)
{
  return (this->UnregisterModule(Log::ToString(module_)));
}

bool
Manager::RegisterConnector(Log::LEVEL level_, Connector* conn_)
{
  bool status = false;
  if ((level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && conn_ && this->_lock.Lock())
  {
    int lfirst = level_;
    int llast = level_;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      this->_conn[l] = conn_;
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

bool
Manager::UnregisterConnector(Log::LEVEL level_)
{
  bool status = false;
  if ((level_ >= Log::LEVEL_ALL)
      && (level_ < Log::LEVEL_LAST) && this->_lock.Lock())
  {
    int lfirst = level_;
    int llast = level_;

    if (level_ != Log::LEVEL_ALL)
    {
      lfirst = level_;
      llast = (lfirst + 1);
    }

    for (int l = lfirst; l < llast; l++)
    {
      this->_conn[l] = NULL;
    }
    status = this->_lock.Unlock();
  }
  return (status);
}

void
Manager::logMessage(const SHARED_PTR(zLog::Message)& message_)
{
  this->_msg_queue.Push(message_);
}

void
Manager::Run(zThread::ThreadArg *arg_)
{

  pthread_t tid = pthread_self();

  while (!this->Exit())
  {

    if (this->_msg_queue.TimedWait(100) && this->_lock.Lock())
    {

      SHARED_PTR(Message)msg = this->_msg_queue.Front();
      this->_msg_queue.Pop();

      if (msg)
      {
        const std::string& module = msg->GetModule();
        Log::LEVEL level = msg->GetLevel();

        if ((this->_max_level.count(module)) && (level <= this->_max_level[module]))
        {
          if (this->_conn[level])
          {
            std::stringstream ss;
            ss << msg->GetTimestamp() << "\t";
            ss << msg->GetProcessId() << "\t";
            ss << msg->GetThreadId() << "\t";
            ss << msg->GetModule() << "\t";
            ss << Log::ToString(msg->GetLevel()) << "\t";
            ss << msg->GetFile() << "[" << msg->GetLine() << "]\t";
            ss << msg->GetMessage() << "\t";
            this->_conn[level]->Logger(ss.str());
          }
        }
      }

      this->_lock.Unlock();
    }

  }

  return;
}

Log::LEVEL
Manager::GetMaxLevel(const std::string& module_)
{
  Log::LEVEL level = Log::LEVEL_DEF;
  if (this->_max_level.count(module_))
  {
    level = this->_max_level[module_];
  }
  return (level);
}

Log::LEVEL
Manager::GetMaxLevel(const Log::MODULE module_)
{
  return (this->GetMaxLevel(Log::ToString(module_)));
}

void
Manager::SetMaxLevel(const std::string& module_, const Log::LEVEL level_)
{
  if (this->_max_level.count(module_) && (level_ > Log::LEVEL_ALL) &&
      (level_ < Log::LEVEL_LAST))
  {
    this->_max_level[module_] = level_;
  }
}

void
Manager::SetMaxLevel(const Log::MODULE module_, const Log::LEVEL level_)
{
  this->SetMaxLevel(Log::ToString(module_), level_);
}

}
}