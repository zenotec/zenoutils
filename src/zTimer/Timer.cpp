//*****************************************************************************
//    Copyright (C) 2016 ZenoTec LLC (http://www.zenotec.net)
//
//    File:
//    Description:
//
//*****************************************************************************

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <mutex>

#include <zutils/zTimer.h>
#include <zutils/zLog.h>

namespace zUtils
{
namespace zTimer
{

static void
_add_time(struct timespec *ts_, uint32_t us_)
{
  // Compute seconds
  ts_->tv_sec += (us_ / 1000000);
  // Compute nanoseconds
  uint32_t nsec = ((us_ % 1000000) * 1000);
  if ((ts_->tv_nsec + nsec) < ts_->tv_nsec)
  {
    ts_->tv_sec++;
  } // end if
  ts_->tv_nsec += nsec;
}

//**********************************************************************
// zTimer::Timer Class
//**********************************************************************

Timer::Timer() :
    _interval(0), _sigev(NULL), _timerid(0), _tick(0), _event(zEvent::Event::TYPE_TIMER)
{

  this->RegisterEvent(&this->_event);

  // Create timer
  this->_sigev = static_cast<struct sigevent *>(calloc(1, sizeof(struct sigevent)));
  if (!this->_sigev)
  {
    ZLOG_CRIT("Cannot create timer: " + std::string(strerror(errno)));
    throw;
  } // end if
  this->_sigev->sigev_notify = SIGEV_THREAD;
  this->_sigev->sigev_notify_function = this->timer_handler;
  this->_sigev->sigev_notify_attributes = NULL;
  this->_sigev->sigev_value.sival_int = 0;
  this->_sigev->sigev_value.sival_ptr = this;
  int stat = timer_create(CLOCK_REALTIME, this->_sigev, &this->_timerid);
  if (stat != 0)
  {
    ZLOG_CRIT("Cannot create timer: " + std::string(strerror(errno)));
    throw;
  } // end if
  this->_lock.Unlock();
}

Timer::~Timer()
{
  this->Stop();
  this->_lock.TimedLock(100000);
  if (this->_timerid)
  {
    int stat = timer_delete(this->_timerid);
    if (stat != 0)
    {
      ZLOG_ERR("Cannot delete timer: " + std::string(strerror(errno)));
    } // end if
    free(this->_sigev);
  } // end if
}

void
Timer::Start(uint32_t usec_)
{
  if (this->_lock.Lock())
  {
    this->_interval = usec_;
    this->_start();
    this->_lock.Unlock();
  } // end if
}

void
Timer::Stop(void)
{
  if (this->_lock.Lock())
  {
    this->_stop();
    this->_lock.Unlock();
  } // end if
}

void
Timer::timer_handler(union sigval sv_)
{
  Timer *self = (Timer *) sv_.sival_ptr;
  if (self->_lock.TryLock())
  {
    self->_tick++;
    TimerNotification notification(self->_tick);
    self->_event.Notify(&notification);
    self->_lock.Unlock();
  }
  pthread_detach(pthread_self());
  pthread_exit(0);
}

void
Timer::_start()
{
  // Compute time
  struct itimerspec its = { 0 };
  _add_time(&its.it_value, this->_interval);
  _add_time(&its.it_interval, this->_interval);
  // Start timer
  int stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot start timer: " + std::string(strerror(errno)));
  } // end if
}

void
Timer::_stop()
{
  // Compute time
  struct itimerspec its = { 0 };
  // Start timer
  int stat = timer_settime(this->_timerid, 0, &its, NULL);
  if (stat != 0)
  {
    ZLOG_ERR("Cannot stop timer: " + std::string(strerror(errno)));
  } // end if
}

//**********************************************************************
// zTimer::TimerNotification Class
//**********************************************************************

TimerNotification::TimerNotification(const uint64_t tick_) :
    _tick(tick_)
{
}

TimerNotification::~TimerNotification()
{
}

uint64_t
TimerNotification::Tick()
{
  return(this->_tick);
}

}
}
