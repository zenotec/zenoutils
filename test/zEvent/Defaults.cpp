#include "zEventTest.h"

int
zEventTest_Defaults(void * arg_)
{
  // Create new event and validate
  zEvent::Event *MyEvent = new zEvent::Event(zEvent::Event::TYPE_TEST);
  TEST_ISNOT_NULL(MyEvent);
  TEST_EQ(zEvent::Event::TYPE_TEST, MyEvent->Type());

  // Create new event handler and validate
  zEvent::EventHandler *MyHandler = new zEvent::EventHandler;
  TEST_ISNOT_NULL(MyHandler);

  // Get instance of event manager
  zEvent::EventManager &manager = zEvent::EventManager::Instance();

  // Cleanup
  delete (MyHandler);
  delete (MyEvent);

  // Return success
  return (0);
}

