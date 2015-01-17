#include "zutils/zLog.h"
#include "zNodeTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zNodeTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::INFO);

  // Test all classes
  UTEST_INIT();
  UTEST_TEST(zNodeTest_NodeDefaults, 0);
  UTEST_TEST(zNodeTest_NodeTableDefaults, 0);
  UTEST_TEST(zNodeTest_NodeGetSet, 0);
  UTEST_TEST(zNodeTest_NodeTableGetSet, 0);
  UTEST_FINI();

  // Exit
  exit(0);

}