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

#include <zutils/zLog.h>
#include <zutils/zData.h>

#include "UnitTest.h"
#include "zDataTest.h"

using namespace zUtils;
using namespace Test;

int
zDataTest_DataCopy(void* arg)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_DataCopy()");
  ZLOG_DEBUG("#############################################################");

  // Create new data object and validate
  std::string key1 = "key1";
  zData::Data *myData1 = new zData::Data(key1);
  TEST_EQ(myData1->Key(), key1);

  // Create new data object and validate
  std::string key2 = "key2";
  zData::Data *myData2 = new zData::Data(key2);
  TEST_EQ(myData2->Key(), key2);

  // Verify data is NOT equal
  TEST_TRUE(*myData1 != *myData2);

  // Copy
  *myData2 = *myData1;

  // Verify data is equal
  TEST_TRUE(*myData1 == *myData2);

  // Clean up
  delete (myData1);
  delete (myData2);

  // Return success
  return (0);

}

