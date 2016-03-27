#include <string>
#include <list>
#include <mutex>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>
#include <zutils/zSwitch.h>

#include <zutils/zGpio.h>

#include "zGpioTest.h"

using namespace zUtils;
using namespace Test;

int
zGpioTest_ConfigurationDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_ConfigurationDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO port configuration and verify
  zGpio::GpioConfiguration *MyConfig = new zGpio::GpioConfiguration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_EQ(MyConfig->Identifier(), zGpio::GpioConfiguration::ConfigIdentifierDefault);
  TEST_EQ(MyConfig->ExportFilename(), zGpio::GpioConfiguration::ConfigExportFilenameDefault);
  TEST_EQ(MyConfig->UnexportFilename(), zGpio::GpioConfiguration::ConfigUnexportFilenameDefault);
  TEST_EQ(MyConfig->DirectionFilename(), zGpio::GpioConfiguration::ConfigDirectionFilenameDefault);
  TEST_EQ(MyConfig->Direction(), zGpio::GpioConfiguration::ConfigDirectionValueDefault);
  TEST_EQ(MyConfig->StateFilename(), zGpio::GpioConfiguration::ConfigStateFilenameDefault);
  TEST_EQ(MyConfig->State(), zGpio::GpioConfiguration::ConfigStateValueDefault);
  TEST_EQ(MyConfig->EdgeFilename(), zGpio::GpioConfiguration::ConfigEdgeFilenameDefault);
  TEST_EQ(MyConfig->Edge(), zGpio::GpioConfiguration::ConfigEdgeValueDefault);

  // Clean up
  delete (MyConfig);
  return (0);
}

int
zGpioTest_PortDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_PortDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO Port test configuration
  TestPortConfig *MyConfig = new TestPortConfig(1);
  TEST_ISNOT_NULL(MyConfig);

  // Create new GPIO port and verify
  zGpio::GpioPort *MyPort = new zGpio::GpioPort(*MyConfig);
  TEST_ISNOT_NULL(MyPort);

  // Clean up
  delete (MyPort);
  delete (MyConfig);
  return (0);
}

int
zGpioTest_HandlerDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_HandlerDefaults()");
  ZLOG_DEBUG("#############################################################");

  // Create new GPIO handler and verify
  zGpio::GpioHandler *myHandler = new zGpio::GpioHandler;
  TEST_ISNOT_NULL(myHandler);

  // Clean up
  delete (myHandler);
  return (0);
}

int
zGpioTest_SwitchDefaults(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zGpioTest_SwitchDefaults()");
  ZLOG_DEBUG("#############################################################");

//    // Create new Switch and verify
//    zGpio::Switch* mySwitch = new zGpio::Switch( zSwitch::Switch::STATE_OFF );
//    TEST_ISNOT_NULL( mySwitch );
//    TEST_FALSE( mySwitch->IsOn() );
//    TEST_TRUE( mySwitch->IsOff() );
//    TEST_IS_NULL( mySwitch->GetPort( 1 ) );

// Clean up
//    delete (mySwitch);
  return (0);
}
