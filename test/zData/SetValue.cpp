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

int
zDataTest_PutValueSingle(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_SetValueSingle()");
  ZLOG_DEBUG("#############################################################");

  std::string expKey = "TestKey";
  std::string expVal = "TestValue";
  std::string obsVal;
  std::string expJson = "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n";
  std::string expXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>";

  // Create new data object and validate
  zData::Data *MyData = new zData::Data(expKey);
  TEST_ISNOT_NULL(MyData);
  TEST_EQ(MyData->Key(), expKey);
  TEST_EQ(MyData->GetJson(), expJson);
  TEST_EQ(MyData->GetXml(), expXml);

  // Set value and validate
  TEST_TRUE(MyData->Put(expVal, expKey));
  TEST_TRUE(MyData->Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}

int
zDataTest_PutValueMultiple(void* arg_)
{

  ZLOG_DEBUG("#############################################################");
  ZLOG_DEBUG("# zDataTest_SetValueMultiple()");
  ZLOG_DEBUG("#############################################################");

  std::string expKey = "TestKey";
  std::string expVal = "TestValue";
  std::string obsVal;
  std::string expJson = "{\n    \"Key\": \"TestKey\",\n    \"TestKey\": \"\"\n}\n";
  std::string expXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Key>TestKey</Key><TestKey/>";

  // Create new data object and validate
  zData::Data *MyData = new zData::Data(expKey);
  TEST_EQ(MyData->Key(), expKey);
  TEST_EQ(MyData->GetJson(), expJson);
  TEST_EQ(MyData->GetXml(), expXml);

  // Set first value and validate
  expKey = "Employee.Name.First";
  expVal = "Elvis";
  TEST_TRUE(MyData->Put(expVal, expKey));
  TEST_TRUE(MyData->Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Set second value and validate
  expKey = "Employee.Name.Last";
  expVal = "Presley";
  TEST_TRUE(MyData->Put(expVal, expKey));
  TEST_TRUE(MyData->Get(obsVal, expKey));
  TEST_EQ(obsVal, expVal);

  // Cleanup
  delete (MyData);

  // Return success
  return (0);
}
