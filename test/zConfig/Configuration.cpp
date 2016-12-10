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

#include "UnitTest.h"
#include "zConfTest.h"

using namespace Test;
using namespace zUtils;

int
zConfigTest_ConfigurationCtor(void* arg_)
{

  // Create new data item and verify
  zData::Data *MyData = new zData::Data("employee");
  TEST_ISNOT_NULL(MyData);

  // Setup data item and verify
  std::string key = "name";
  std::string val = "Elvis";
  std::string obs = "";
  TEST_TRUE(MyData->Put(val, key));
  TEST_TRUE(MyData->Get(obs, key));
  TEST_EQ(obs, val);

  // Create const configuration data item from data item and verify
  const zConfig::Configuration ConstConfig(*MyData);
  TEST_FALSE(ConstConfig.IsModified());

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration(ConstConfig);
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  zData::Data ObsData;

  TEST_TRUE(MyConfig->Get(ObsData, "employee"));
  TEST_TRUE(ObsData == *MyData);
  TEST_TRUE(ObsData.Get(obs, key));
  TEST_EQ(obs, val);

  return (0);

}

int
zConfigTest_ConfigurationGetSetData(void* arg_)
{
  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  // Create new data item and verify
  zData::Data *MyData = new zData::Data("employee");
  TEST_ISNOT_NULL(MyData);

  // Setup data item and verify
  std::string key = "name";
  std::string val = "Elvis";
  std::string obs = "";
  TEST_TRUE(MyData->Put(val, key));
  TEST_TRUE(MyData->Get(obs, key));
  TEST_EQ(obs, val);

  zData::Data ConfData;
  zData::Data ObsData;

  // Verify the key doesn't exist and isn't set
  TEST_TRUE(MyConfig->Get(ConfData));
  TEST_FALSE(ConfData.Get(ObsData, "employee"));

  // Set the data and verify (only updates staging data)
  TEST_TRUE(MyConfig->Get(ConfData));
  TEST_TRUE(ConfData.Put(*MyData, "employee"));
  TEST_TRUE(MyConfig->Put(ConfData));
  TEST_TRUE(MyConfig->Get(ConfData));
  TEST_FALSE(ConfData.Get(ObsData, "employee"));

  // Commit data and verify
  TEST_TRUE(MyConfig->Commit());
  TEST_TRUE(MyConfig->Get(ConfData));
  TEST_TRUE(ConfData.Get(ObsData, "employee"));
  TEST_TRUE(ObsData.Get(obs, key));
  TEST_EQ(obs, val);

  // Cleanup
  delete (MyData);
  delete (MyConfig);

  // Return success
  return (0);

}

int
zConfigTest_ConfigurationCompare(void* arg_)
{
  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig1 = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig1);
  TEST_FALSE(MyConfig1->IsModified());

  // Create second configuration data item and verify
  zConfig::Configuration *MyConfig2 = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig2);
  TEST_FALSE(MyConfig2->IsModified());

  // Verify both are equal
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  zData::Data MyData("employee");
  std::string key = "name";
  std::string val = "Elvis";
  TEST_TRUE(MyData.Put(val, key));

  // Modify first configuration data item
  TEST_TRUE(MyConfig1->Put(MyData));
  TEST_TRUE(MyConfig1->IsModified());

  // Verify both are still equal
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  // Commit data and verify they are no longer equal
  TEST_TRUE(MyConfig1->Commit());
  TEST_FALSE(MyConfig1->IsModified());
  TEST_TRUE(MyConfig2->Commit());
  TEST_FALSE(MyConfig2->IsModified());
  TEST_FALSE(*MyConfig1 == *MyConfig2);
  TEST_TRUE(*MyConfig1 != *MyConfig2);

  // Modify second configuration data item
  TEST_TRUE(MyConfig2->Put(MyData));
  TEST_TRUE(MyConfig2->IsModified());

  // Verify they are still not equal
  TEST_FALSE(*MyConfig1 == *MyConfig2);
  TEST_TRUE(*MyConfig1 != *MyConfig2);

  // Commit and verify both are equal again
  TEST_TRUE(MyConfig1->Commit());
  TEST_FALSE(MyConfig1->IsModified());
  TEST_TRUE(MyConfig2->Commit());
  TEST_FALSE(MyConfig2->IsModified());
  TEST_TRUE(*MyConfig1 == *MyConfig2);
  TEST_FALSE(*MyConfig1 != *MyConfig2);

  // Cleanup
  delete (MyConfig2);
  delete (MyConfig1);

  // Return success
  return (0);

}
int
zConfigTest_ConfigurationDataArray(void* arg_)
{

  // Create new configuration data item and verify
  zConfig::Configuration *MyConfig = new zConfig::Configuration;
  TEST_ISNOT_NULL(MyConfig);
  TEST_FALSE(MyConfig->IsModified());

  // Create new data item and verify
  zData::Data *MyData1 = new zData::Data("employee");
  TEST_ISNOT_NULL(MyData1);

  // Setup data item and verify
  std::string key = "name";
  std::string val1 = "Moe";
  std::string obs1 = "";
  TEST_TRUE(MyData1->Put(val1, key));
  TEST_TRUE(MyData1->Get(obs1, key));
  TEST_EQ(obs1, val1);

  // Create new data item and verify
  zData::Data *MyData2 = new zData::Data("employee");
  TEST_ISNOT_NULL(MyData2);

  // Setup data item and verify
  std::string val2 = "Larry";
  std::string obs2 = "";
  TEST_TRUE(MyData2->Put(val2, key));
  TEST_TRUE(MyData2->Get(obs2, key));
  TEST_EQ(obs2, val2);

  // Create new data item and verify
  zData::Data *MyData3 = new zData::Data("employee");
  TEST_ISNOT_NULL(MyData3);

  // Setup data item and verify
  std::string val3 = "Curly";
  std::string obs3 = "";
  TEST_TRUE(MyData3->Put(val3, key));
  TEST_TRUE(MyData3->Get(obs3, key));
  TEST_EQ(obs3, val3);

  zData::Data ObsData;

  // Add the data items and verify (only updates staging data)
  TEST_TRUE(MyConfig->Add(*MyData1, MyData1->Key()));
  TEST_TRUE(MyConfig->IsModified());
  TEST_TRUE(MyConfig->Add(*MyData2, MyData2->Key()));
  TEST_TRUE(MyConfig->IsModified());
  TEST_TRUE(MyConfig->Add(*MyData3, MyData3->Key()));
  TEST_TRUE(MyConfig->IsModified());
  //MyConfig->Display();

  // Commit data and verify
  TEST_TRUE(MyConfig->Commit());
  TEST_FALSE(MyConfig->IsModified());
  //MyConfig->Display();
  TEST_TRUE(MyConfig->Get(ObsData, "employee"));

  std::unique_ptr<zData::Data> d;

  d = ObsData[0];
  TEST_FALSE(d == NULL);
  TEST_TRUE(d->Get(obs1, key));
  TEST_EQ(obs1, val1);

  d = ObsData[1];
  TEST_FALSE(d == NULL);
  TEST_TRUE(d->Get(obs2, key));
  TEST_EQ(obs2, val2);

  d = ObsData[2];
  TEST_FALSE(d == NULL);
  TEST_TRUE(d->Get(obs3, key));
  TEST_EQ(obs3, val3);

  // Cleanup
  delete (MyData1);
  delete (MyData2);
  delete (MyData3);
  delete (MyConfig);

  // Return success
  return (0);
}
