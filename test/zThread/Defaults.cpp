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

#include <list>
#include <mutex>

#include "zThreadTest.h"

using namespace Test;
using namespace zUtils;

int
zThreadTest_Defaults(void* arg_)
{

  // Create test thread and validate
  TestThread *myThread = new TestThread("Defaults", 0);
  TEST_ISNOT_NULL(myThread);
  TEST_EQ(std::string("Defaults"), myThread->Name());
  TEST_IS_ZERO(myThread->TestArg.LoopCnt);
  delete (myThread);
  return (0);
}

