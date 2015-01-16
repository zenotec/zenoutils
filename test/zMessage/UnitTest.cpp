#include "zutils/zLog.h"
#include "zMessageTest.h"
#include "UnitTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zMessageTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zMessageTest_Defaults, 0);
  UTEST_TEST(zMessageTest_HelloDefaults, 0);
  UTEST_TEST(zMessageTest_AckDefaults, 0);
  UTEST_TEST(zMessageTest_ByeDefaults, 0);
  UTEST_TEST(zMessageTest_CfgDefaults, 0);
  UTEST_TEST(zMessageTest_CmdDefaults, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}
