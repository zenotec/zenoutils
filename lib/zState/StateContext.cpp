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

// libc includes


// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/zEvent.h>
#include <zutils/zState.h>

// local includes

ZLOG_MODULE_INIT(zLog::Log::MODULE_STATE);

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: Context
//**********************************************************************

Context::Context() :
    zEvent::Event(zEvent::TYPE_STATE)
{
  this->_last = NULL;
  this->_state = NULL;
  this->_next = NULL;
  this->_lock.Unlock();
}

Context::~Context()
{
  this->_lock.Lock();
  // Explicitly delete all states before the context gets destructed
  this->_next = NULL;
  this->_state = NULL;
  this->_last = NULL;
}

uint32_t
Context::GetLastStateId() const
{
  uint32_t id = 0;
  SHPTR(zState::State)s(this->GetLastState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHPTR(zState::State)
Context::GetLastState() const
{
  SHPTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_last;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetLastState(SHPTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_last = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

uint32_t
Context::GetStateId() const
{
  uint32_t id = 0;
  SHPTR(zState::State)s(this->GetState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHPTR(zState::State)
Context::GetState() const
{
  SHPTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_state;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetState(SHPTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_state = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

uint32_t
Context::GetNextStateId() const
{
  uint32_t id = 0;
  SHPTR(zState::State) s(this->GetNextState());
  if (s.get())
  {
    id = s->GetId();
  }
  return (id);
}

SHPTR(zState::State)
Context::GetNextState() const
{
  SHPTR(zState::State) s;
  if (this->_lock.Lock())
  {
    s = this->_next;
    this->_lock.Unlock();
  }
  return (s);
}

bool
Context::SetNextState(SHPTR(zState::State) state_)
{
  bool status = false;
  if (this->_lock.Lock())
  {
    this->_next = state_;
    status = this->_lock.Unlock();
  }
  return (status);
}

zEvent::STATUS
Context::SetNextStateAndNotify(SHPTR(zState::State) state_, const uint32_t cmd_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;
  if (this->SetNextState(state_))
  {
    status = this->Notify(cmd_);
  }
  return (status);
}

zEvent::STATUS
Context::SetNextStateAndNotify(SHPTR(zState::State) state_, SHPTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;
  if (this->SetNextState(state_))
  {
    status = this->Notify(n_);
  }
  return (status);
}

zEvent::STATUS
Context::Notify(const uint32_t cmd_)
{
  SHPTR(zState::Notification) n(new zState::Notification(*this));
  n->SetCommand(cmd_);
  return (this->Notify(n));
}

zEvent::STATUS
Context::Notify(SHPTR(zEvent::Notification) n_)
{
  zEvent::STATUS status = zEvent::STATUS_ERR;

  // Hold lock while state is observing event to block other threads from trying to modify the state
  //    note: lock allows same thread to relock the lock while blocking other threads from the same
  if (this->_lock.Lock())
  {

    // Protect current state from destroying itself by notifying next state
    SHPTR(zState::State) s(this->GetNextState());
    if (s.get())
    {
      // Advance state
      this->SetLastState(this->GetState());
      this->SetState(this->GetNextState());

      ZLOG_DEBUG("State change: " + ZLOG_UINT(this->GetLastStateId()) + " -> " + ZLOG_UINT(this->GetStateId()));
      status = this->GetState()->ObserveEvent(n_);
      ZLOG_DEBUG("State status: " + ZLOG_UINT(status));
    }
    else
    {
      ZLOG_ERR("NULL state");
    }

    this->_lock.Unlock();
  }

  return (status);
}

zEvent::STATUS
Context::ObserveEvent(SHPTR(zEvent::Notification) n_)
{
  return (this->Notify(n_));
}

}
}
