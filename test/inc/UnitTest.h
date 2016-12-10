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

#ifndef _UTEST_H_
#define _UTEST_H_

//*****************************************************************************
//  Includes
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <string>
#include <iostream>
#include <sstream>
#include <queue>

//*****************************************************************************
//  Defines
//*****************************************************************************

#define DEBUG        1

#define TIMELOG_LEN     10000

#define TIMELOG_START   1
#define TIMELOG_STOP    2

//*****************************************************************************
//  Macros
//*****************************************************************************

#if DEBUG

#define DPRINTF(fmt, ...) \
{ \
   fprintf( stdout, "%s:%d: ", __FILE__, __LINE__ ); \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#else
#define DPRINTF(fmt, ...)
#endif

#define IPRINTF(fmt, ...) \
{ \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#define EPRINTF(fmt, ...) \
{ \
   fprintf( stdout, "%s:%d\n", __FILE__, __LINE__ ); \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#define DOTS(n) \
{ \
   for (int i = 0; i < n; i++) IPRINTF("."); \
}

#define UTEST_INIT() \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest ) \
  { \
    utest->SetName( std::string(__FUNCTION__) ); \
    utest->DisplayHeader(); \
  } \
} while(false);

#define UTEST_FINI() \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest ) \
  { \
    utest->DisplayFooter(); \
  } \
  return( utest->GetStatus()); \
} while(false);

#define UTEST_TEST(fnc, arg) \
do { \
   int status = 0; \
   IPRINTF( "  %s()", #fnc ); \
   DOTS((50-sizeof(#fnc))); \
   if ( !( status = (fnc)(arg) ) ) \
   { \
      IPRINTF( "success\n" ); \
   } \
   else \
   { \
      IPRINTF("fail: %d\n", status); \
      Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
      if(utest) \
      { \
        utest->SetStatus(status); \
        utest->DisplayLog(); \
      } \
   } \
} while(false)

#define TEST_IS_NULL(x) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Eq<void*>( __LINE__, (void*)(x), (void*)NULL, std::string( "" ) ) != true ) \
      return (1); \
} while(false)

#define TEST_ISNOT_NULL(x) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Neq<void*>( __LINE__, (x), NULL, std::string( "" ) ) != true ) \
      return (1); \
} while(false)

#define TEST_IS_ZERO(x) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Eq<int>( __LINE__, (x), 0, std::string( "" ) ) != true ) \
      return (1); \
} while(false)

#define TEST_ISNOT_ZERO(x) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Neq<int>( __LINE__, (x), 0, std::string( "" ) ) != true ) \
      return (2); \
} while(false)

#define TEST_TRUE(x) TEST_TRUE_MSG(x, std::string( "" ))
#define TEST_TRUE_MSG(x, msg) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->True( __LINE__, (x), msg ) != true ) \
      return (1); \
} while(false)

#define TEST_FALSE(x) TEST_FALSE_MSG(x, std::string( "" ))
#define TEST_FALSE_MSG(x, msg) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->False( __LINE__, (x), msg ) != true ) \
      return (2); \
} while(false)

#define TEST_EQ(x, y) TEST_EQ_MSG(x, y, std::string( "" ))
#define TEST_EQ_MSG(x, y, msg) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Eq<typeof(x)>( __LINE__, (x), (y), (msg) ) != true ) \
    return (3); \
} while(false)

#define TEST_NEQ(x, y) TEST_NEQ_MSG(x, y, std::string( "" ))
#define TEST_NEQ_MSG(x, y, msg) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Neq<typeof(x)>( __LINE__, (x), (y), (msg) ) != true ) \
    return (4); \
} while(false)

#define TEST_LT(x, y) TEST_LT_MSG(x, y, std::string( "" ))
#define TEST_LT_MSG(x, y, msg) \
do { \
  UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Lt<typeof(x)>( __LINE__, (x), (y), (msg) ) != true ) \
    return (5); \
} while(false)

#define TEST_GT(x, y) TEST_GT_MSG((x), (y), std::string( "" ) )
#define TEST_GT_MSG(x, y, msg) \
do { \
  Test::UnitTest* utest = Test::UnitTest::GetInstance(); \
  if( utest && utest->Gt<typeof(x)>( __LINE__, (x), (y), (msg) ) != true ) \
    return (6); \
} while(false)

//*****************************************************************************
//  Type Definitions
//*****************************************************************************

//*****************************************************************************
//  Globals
//*****************************************************************************

//*****************************************************************************
//  Classes
//*****************************************************************************

namespace Test
{

class Log
{
public:
  Log(const unsigned int line_, const std::string &msg_) :
      _line(line_), _msg(msg_)
  {
    clock_gettime(CLOCK_REALTIME, &this->_timestamp);
  }

  virtual
  ~Log()
  {
  }

  std::string
  GetTimeStamp()
  {
    std::stringstream timestamp;
    timestamp << this->_timestamp.tv_sec << "." << this->_timestamp.tv_nsec;
    return (timestamp.str());
  }

  int
  GetLine()
  {
    return (this->_line);
  }

  std::string
  GetMsg()
  {
    return (this->_msg);
  }

protected:

private:
  struct timespec _timestamp;
  int _line;
  std::string _msg;
};

class UnitTest
{

public:

  static UnitTest*
  GetInstance()
  {
    static UnitTest utest;
    return &utest;
  }

  std::string
  GetName()
  {
    return (this->_name);
  }

  void
  SetName(const std::string &name_)
  {
    this->_name = name_;
  }

  // Boolean test
  bool
  True(const int line_, const bool &val_, const std::string &msg_)
  {
    if (val_ != true)
    {
      std::stringstream errmsg;
      errmsg << "Not true" << std::endl << msg_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  False(const int line_, const bool &val_, const std::string &msg_)
  {
    if (val_ != false)
    {
      std::stringstream errmsg;
      errmsg << "Not false" << std::endl << msg_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }

  // Comparison

  template<typename T>
    bool
    Eq(const int line_, T i1_, T i2_, const std::string &msg_)
    {
      if (i1_ != i2_)
      {
        std::stringstream errmsg;
        errmsg << i1_ << " != " << i2_ << std::endl << msg_ << std::endl;
        this->LogMsg(line_, errmsg.str());
        return (false);
      } // end if
      else
      {
        return (true);
      } // end else
    }

  template<typename T>
    bool
    Neq(const int line_, T i1_, T i2_, const std::string &msg_)
    {
      if (i1_ == i2_)
      {
        std::stringstream errmsg;
        errmsg << i1_ << " == " << i2_ << std::endl << msg_ << std::endl;
        this->LogMsg(line_, errmsg.str());
        return (false);
      } // end if
      else
      {
        return (true);
      } // end else
    }

  template<typename T>
    bool
    Lt(const int line_, T i1_, T i2_, const std::string &msg_)
    {
      if (i1_ >= i2_)
      {
        std::stringstream errmsg;
        errmsg << i1_ << " >= " << i2_ << std::endl << msg_ << std::endl;
        this->LogMsg(line_, errmsg.str());
        return (false);
      } // end if
      else
      {
        return (true);
      } // end else
    }

  template<typename T>
    bool
    Gt(const int line_, T i1_, T i2_, const std::string &msg_)
    {
      if (i1_ <= i2_)
      {
        std::stringstream errmsg;
        errmsg << i1_ << " <= " << i2_ << std::endl << msg_ << std::endl;
        this->LogMsg(line_, errmsg.str());
        return (false);
      } // end if
      else
      {
        return (true);
      } // end else
    }

  void
  LogMsg(const int line_, const std::string &msg_)
  {
    Log log(line_, msg_);
    this->_log.push(log);
  }

  void
  DisplayHeader()
  {
    clock_gettime(CLOCK_REALTIME, &this->_start);
    IPRINTF("\nExecuting unit tests: %s\n", this->GetName().c_str());
  }

  void
  DisplayFooter()
  {
    double diff = 0;
    clock_gettime(CLOCK_REALTIME, &this->_stop);
    diff = (this->_stop.tv_sec - this->_start.tv_sec)
        + (double) (this->_stop.tv_nsec - this->_start.tv_nsec) / (double) 1000000000ULL;
    IPRINTF("Complete: %lf seconds\n", diff);
  }

  void
  DisplayLog()
  {
    std::cout.flush();
    std::cerr.flush();
    while (!this->_log.empty())
    {
      std::cerr << this->_log.front().GetTimeStamp() << ": ";
      std::cerr << "LINE #";
      std::cerr << this->_log.front().GetLine() << ": ";
      std::cerr << this->_log.front().GetMsg() << std::endl;
      std::cerr.flush();
      this->_log.pop();
    } // end while
    std::cout.flush();
    std::cerr.flush();
  }

  int
  GetStatus()
  {
    return (this->_status);
  }

  void
  SetStatus(int status_)
  {
    this->_status = status_;
  }

protected:
  UnitTest() :
      _status(0)
  {
  }

  virtual
  ~UnitTest()
  {
  }

private:

  std::string _name;
  int _status;
  struct timespec _start;
  struct timespec _stop;
  std::queue<Log> _log;

};

}

#endif /* _UTEST_H_ */
